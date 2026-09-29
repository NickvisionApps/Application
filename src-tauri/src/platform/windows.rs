use raw_window_handle::{HasWindowHandle, RawWindowHandle};
use std::{
    collections::HashMap,
    sync::{Mutex, OnceLock},
};
use tauri::{Emitter, WebviewWindow, Wry};
use windows_sys::Win32::{
    Foundation::{HINSTANCE, HWND, LPARAM, LRESULT, WPARAM},
    System::LibraryLoader::GetModuleHandleW,
    UI::{
        HiDpi::GetDpiForWindow,
        Input::KeyboardAndMouse::{TME_LEAVE, TME_NONCLIENT, TRACKMOUSEEVENT, TrackMouseEvent},
        Shell::{DefSubclassProc, RemoveWindowSubclass, SetWindowSubclass},
        WindowsAndMessaging::{
            CS_HREDRAW, CS_VREDRAW, CreateWindowExW, DefWindowProcW, DestroyWindow, GetClientRect,
            HTMAXBUTTON, HWND_TOP, RegisterClassExW, SW_HIDE, SWP_NOACTIVATE, SWP_SHOWWINDOW,
            SetWindowPos, ShowWindow, WM_DPICHANGED, WM_NCDESTROY, WM_NCHITTEST, WM_NCLBUTTONDOWN,
            WM_NCLBUTTONUP, WM_NCMOUSELEAVE, WM_NCMOUSEMOVE, WM_SIZE, WNDCLASSEXW, WS_CHILD,
            WS_CLIPSIBLINGS, WS_VISIBLE,
        },
    },
};

const MAX_TITLEBAR_BAND_LOGICAL: u64 = 128;
const MAX_CONTROL_BAND_LOGICAL: u64 = 640;
const MAX_MAXIMIZE_WIDTH_LOGICAL: u64 = 256;
const MAX_MAXIMIZE_HEIGHT_LOGICAL: u64 = 128;
const SUBCLASS_ID: usize = 1;

const SNAP_CLASS: &[u16] = &[
    b'A' as u16,
    b'p' as u16,
    b'p' as u16,
    b'l' as u16,
    b'i' as u16,
    b'c' as u16,
    b'a' as u16,
    b't' as u16,
    b'i' as u16,
    b'o' as u16,
    b'n' as u16,
    b'S' as u16,
    b'n' as u16,
    b'a' as u16,
    b'p' as u16,
    0,
];

#[derive(Clone, Copy)]
struct Geometry {
    x: i32,
    y: i32,
    width: u32,
    height: u32,
}

struct Entry {
    overlay: isize,
    window: WebviewWindow<Wry>,
    geometry: Geometry,
    hovering: bool,
    pressing: bool,
}

fn registry() -> &'static Mutex<HashMap<isize, Entry>> {
    static REGISTRY: OnceLock<Mutex<HashMap<isize, Entry>>> = OnceLock::new();
    REGISTRY.get_or_init(|| Mutex::new(HashMap::new()))
}

pub fn install_or_update(
    window: &WebviewWindow<Wry>,
    x: i32,
    y: i32,
    width: u32,
    height: u32,
    titlebar_height: u32,
    control_band_width: u32,
) -> Result<(), String> {
    let hwnd = window_hwnd(window)?;
    let geometry = Geometry {
        x,
        y,
        width,
        height,
    };
    validate_geometry(hwnd, geometry, titlebar_height, control_band_width)?;
    if registry().lock().unwrap().contains_key(&hwnd) {
        update_geometry(hwnd, geometry)
    } else {
        install(window, hwnd, geometry)
    }
}

pub fn uninstall(window: &WebviewWindow<Wry>) -> Result<(), String> {
    uninstall_hwnd(window_hwnd(window)?)
}

fn window_hwnd(window: &WebviewWindow<Wry>) -> Result<isize, String> {
    match window
        .window_handle()
        .map_err(|error| error.to_string())?
        .as_raw()
    {
        RawWindowHandle::Win32(handle) => Ok(handle.hwnd.get()),
        _ => Err("native snap overlay requires a Win32 window handle".to_string()),
    }
}

fn validate_geometry(
    hwnd: isize,
    geometry: Geometry,
    titlebar_height: u32,
    control_band_width: u32,
) -> Result<(), String> {
    if geometry.width == 0 || geometry.height == 0 || geometry.x < 0 || geometry.y < 0 {
        return Err(
            "snap maximize-button geometry must be a non-empty client rectangle".to_string(),
        );
    }
    let mut rect = unsafe { std::mem::zeroed() };
    if unsafe { GetClientRect(hwnd as HWND, &mut rect) } == 0 {
        return Err(format!(
            "GetClientRect failed: {}",
            std::io::Error::last_os_error()
        ));
    }
    let right = i64::from(geometry.x) + i64::from(geometry.width);
    let bottom = i64::from(geometry.y) + i64::from(geometry.height);
    if right > i64::from(rect.right) || bottom > i64::from(rect.bottom) {
        return Err(
            "snap maximize-button geometry lies outside the window client area".to_string(),
        );
    }
    let dpi = unsafe { GetDpiForWindow(hwnd as HWND) };
    if dpi == 0 {
        return Err(format!(
            "GetDpiForWindow failed: {}",
            std::io::Error::last_os_error()
        ));
    }
    if !trusted_caption_region(
        rect.right,
        dpi,
        geometry,
        titlebar_height,
        control_band_width,
    ) {
        return Err(
            "snap maximize-button geometry is outside the trusted top-right caption region"
                .to_string(),
        );
    }
    Ok(())
}

fn trusted_caption_region(
    client_right: i32,
    dpi: u32,
    geometry: Geometry,
    titlebar_height: u32,
    control_band_width: u32,
) -> bool {
    let right = i64::from(geometry.x) + i64::from(geometry.width);
    let logical = |value: u64| value.saturating_mul(96).div_ceil(u64::from(dpi));
    let titlebar_height_logical = logical(u64::from(titlebar_height));
    let control_band_width_logical = logical(u64::from(control_band_width));
    if titlebar_height_logical == 0
        || titlebar_height_logical > MAX_TITLEBAR_BAND_LOGICAL
        || control_band_width_logical == 0
        || control_band_width_logical > MAX_CONTROL_BAND_LOGICAL
    {
        return false;
    }
    let width = u64::from(geometry.width);
    let height = u64::from(geometry.height);
    let width_logical = logical(width);
    let height_logical = logical(height);
    if width_logical == 0
        || width_logical > MAX_MAXIMIZE_WIDTH_LOGICAL
        || height_logical == 0
        || height_logical > MAX_MAXIMIZE_HEIGHT_LOGICAL
    {
        return false;
    }
    let top = u64::try_from(geometry.y).unwrap_or(u64::MAX);
    let distance_from_right = u64::try_from(i64::from(client_right) - right).unwrap_or(u64::MAX);
    distance_from_right.saturating_add(width) <= u64::from(control_band_width)
        && height <= u64::from(titlebar_height)
        && top.saturating_add(height) <= u64::from(titlebar_height)
}

fn install(window: &WebviewWindow<Wry>, hwnd: isize, geometry: Geometry) -> Result<(), String> {
    let instance = register_class()?;
    let parent = hwnd as HWND;
    let overlay = unsafe {
        CreateWindowExW(
            0,
            SNAP_CLASS.as_ptr(),
            SNAP_CLASS.as_ptr(),
            WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
            0,
            0,
            0,
            0,
            parent,
            std::ptr::null_mut(),
            instance,
            std::ptr::null_mut(),
        )
    };
    if overlay.is_null() {
        return Err(format!(
            "CreateWindowExW failed: {}",
            std::io::Error::last_os_error()
        ));
    }
    if unsafe { SetWindowSubclass(parent, Some(parent_subclass_proc), SUBCLASS_ID, 0) } == 0 {
        unsafe { DestroyWindow(overlay) };
        return Err("SetWindowSubclass failed for the snap overlay".to_string());
    }
    registry().lock().unwrap().insert(
        hwnd,
        Entry {
            overlay: overlay as isize,
            window: window.clone(),
            geometry,
            hovering: false,
            pressing: false,
        },
    );
    update_overlay_position(hwnd)
}

fn update_geometry(hwnd: isize, geometry: Geometry) -> Result<(), String> {
    let Some(entry) = registry().lock().unwrap().get_mut(&hwnd) else {
        return Err("no snap overlay is installed for this window".to_string());
    };
    entry.geometry = geometry;
    update_overlay_position(hwnd)
}

fn update_overlay_position(hwnd: isize) -> Result<(), String> {
    let Some((overlay, geometry)) = registry()
        .lock()
        .unwrap()
        .get(&hwnd)
        .map(|entry| (entry.overlay, entry.geometry))
    else {
        return Ok(());
    };
    if unsafe {
        SetWindowPos(
            overlay as HWND,
            HWND_TOP,
            geometry.x,
            geometry.y,
            geometry.width as i32,
            geometry.height as i32,
            SWP_NOACTIVATE | SWP_SHOWWINDOW,
        )
    } == 0
    {
        return Err(format!(
            "SetWindowPos failed: {}",
            std::io::Error::last_os_error()
        ));
    }
    Ok(())
}

fn uninstall_hwnd(hwnd: isize) -> Result<(), String> {
    let Some(entry) = registry().lock().unwrap().remove(&hwnd) else {
        return Ok(());
    };
    unsafe {
        ShowWindow(entry.overlay as HWND, SW_HIDE);
        DestroyWindow(entry.overlay as HWND);
        RemoveWindowSubclass(hwnd as HWND, Some(parent_subclass_proc), SUBCLASS_ID);
    }
    Ok(())
}

fn register_class() -> Result<HINSTANCE, String> {
    static REGISTRATION: OnceLock<Result<isize, String>> = OnceLock::new();
    let registration = REGISTRATION.get_or_init(|| unsafe {
        let instance = GetModuleHandleW(std::ptr::null()) as HINSTANCE;
        if instance.is_null() {
            return Err(format!(
                "GetModuleHandleW failed: {}",
                std::io::Error::last_os_error()
            ));
        }
        let class = WNDCLASSEXW {
            cbSize: std::mem::size_of::<WNDCLASSEXW>() as u32,
            style: CS_HREDRAW | CS_VREDRAW,
            lpfnWndProc: Some(overlay_proc),
            cbClsExtra: 0,
            cbWndExtra: 0,
            hInstance: instance,
            hIcon: std::ptr::null_mut(),
            hCursor: std::ptr::null_mut(),
            hbrBackground: std::ptr::null_mut(),
            lpszMenuName: std::ptr::null(),
            lpszClassName: SNAP_CLASS.as_ptr(),
            hIconSm: std::ptr::null_mut(),
        };
        if unsafe { RegisterClassExW(&class) } == 0 {
            return Err(format!(
                "RegisterClassExW failed: {}",
                std::io::Error::last_os_error()
            ));
        }
        Ok(instance as isize)
    });
    registration
        .as_ref()
        .copied()
        .map(|instance| instance as HINSTANCE)
        .map_err(|error| error.clone())
}

fn registered_overlay(hwnd: HWND) -> bool {
    registry()
        .lock()
        .unwrap()
        .values()
        .any(|entry| entry.overlay == hwnd as isize)
}

fn parent_for_overlay(overlay: HWND) -> Option<isize> {
    registry()
        .lock()
        .unwrap()
        .iter()
        .find_map(|(parent, entry)| (entry.overlay == overlay as isize).then_some(*parent))
}

fn emit_snap_event(hwnd: HWND, event: &str, value: bool) {
    let Some(parent) = parent_for_overlay(hwnd) else {
        return;
    };
    let Some(window) = registry()
        .lock()
        .unwrap()
        .get(&parent)
        .map(|entry| entry.window.clone())
    else {
        return;
    };
    let _ = window.emit(event, value);
}

unsafe fn handle_parent_message(hwnd: HWND, msg: u32, wparam: WPARAM, lparam: LPARAM) -> LRESULT {
    match msg {
        WM_SIZE | WM_DPICHANGED => {
            if let Some(entry) = registry().lock().unwrap().get(&(hwnd as isize)) {
                unsafe { ShowWindow(entry.overlay as HWND, SW_HIDE) };
            }
        }
        WM_NCDESTROY => {
            registry().lock().unwrap().remove(&(hwnd as isize));
            unsafe { RemoveWindowSubclass(hwnd, Some(parent_subclass_proc), SUBCLASS_ID) };
        }
        _ => {}
    }
    unsafe { DefSubclassProc(hwnd, msg, wparam, lparam) }
}

unsafe extern "system" fn parent_subclass_proc(
    hwnd: HWND,
    msg: u32,
    wparam: WPARAM,
    lparam: LPARAM,
    _subclass_id: usize,
    _ref_data: usize,
) -> LRESULT {
    std::panic::catch_unwind(std::panic::AssertUnwindSafe(|| unsafe {
        handle_parent_message(hwnd, msg, wparam, lparam)
    }))
    .unwrap_or_else(|_| unsafe { DefSubclassProc(hwnd, msg, wparam, lparam) })
}

unsafe fn handle_overlay_message(hwnd: HWND, msg: u32, wparam: WPARAM, lparam: LPARAM) -> LRESULT {
    match msg {
        WM_NCHITTEST if registered_overlay(hwnd) => return HTMAXBUTTON as LRESULT,
        WM_NCMOUSEMOVE => {
            let Some(parent) = parent_for_overlay(hwnd) else {
                return 0;
            };
            let entered = {
                let Some(entry) = registry().lock().unwrap().get_mut(&parent) else {
                    return 0;
                };
                let entered = !entry.hovering;
                entry.hovering = true;
                entered
            };
            if entered {
                emit_snap_event(hwnd, "snap-hover", true);
                let mut track = TRACKMOUSEEVENT {
                    cbSize: std::mem::size_of::<TRACKMOUSEEVENT>() as u32,
                    dwFlags: TME_LEAVE | TME_NONCLIENT,
                    hwndTrack: hwnd,
                    dwHoverTime: 0,
                };
                unsafe { TrackMouseEvent(&mut track) };
            }
            return 0;
        }
        WM_NCMOUSELEAVE => {
            if let Some(parent) = parent_for_overlay(hwnd) {
                if let Some(entry) = registry().lock().unwrap().get_mut(&parent) {
                    entry.hovering = false;
                    entry.pressing = false;
                }
            }
            emit_snap_event(hwnd, "snap-hover", false);
            return 0;
        }
        WM_NCLBUTTONDOWN => {
            if let Some(parent) = parent_for_overlay(hwnd) {
                if let Some(entry) = registry().lock().unwrap().get_mut(&parent) {
                    entry.pressing = true;
                }
            }
            emit_snap_event(hwnd, "snap-press", true);
            return 0;
        }
        WM_NCLBUTTONUP => {
            if let Some(parent) = parent_for_overlay(hwnd) {
                if let Some(entry) = registry().lock().unwrap().get_mut(&parent) {
                    entry.pressing = false;
                }
            }
            emit_snap_event(hwnd, "snap-press", false);
            return 0;
        }
        WM_NCDESTROY => {
            if let Some(parent) = parent_for_overlay(hwnd) {
                registry().lock().unwrap().remove(&parent);
            }
        }
        _ => {}
    }
    unsafe { DefWindowProcW(hwnd, msg, wparam, lparam) }
}

unsafe extern "system" fn overlay_proc(
    hwnd: HWND,
    msg: u32,
    wparam: WPARAM,
    lparam: LPARAM,
) -> LRESULT {
    std::panic::catch_unwind(std::panic::AssertUnwindSafe(|| unsafe {
        handle_overlay_message(hwnd, msg, wparam, lparam)
    }))
    .unwrap_or_else(|_| unsafe { DefWindowProcW(hwnd, msg, wparam, lparam) })
}
