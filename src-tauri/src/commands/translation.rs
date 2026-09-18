use crate::translation;
use tauri::command;

#[command]
pub fn translate_f(msgid: String, args: Vec<String>) -> String {
    translation::_f(&msgid, &args)
}

#[command]
pub fn translate_g(msgid: String) -> String {
    translation::_g(&msgid)
}

#[command]
pub fn translate_n(msgid: String, msgid_plural: String, n: u64) -> String {
    translation::_n(&msgid, &msgid_plural, n)
}

#[command]
pub fn translate_nf(msgid: String, msgid_plural: String, n: u64, args: Vec<String>) -> String {
    translation::_nf(&msgid, &msgid_plural, n, &args)
}

#[command]
pub fn translate_np(msgctxt: String, msgid: String, msgid_plural: String, n: u64) -> String {
    translation::_np(&msgctxt, &msgid, &msgid_plural, n)
}

#[command]
pub fn translate_npf(
    msgctxt: String,
    msgid: String,
    msgid_plural: String,
    n: u64,
    args: Vec<String>,
) -> String {
    translation::_npf(&msgctxt, &msgid, &msgid_plural, n, &args)
}

#[command]
pub fn translate_p(msgctxt: String, msgid: String) -> String {
    translation::_p(&msgctxt, &msgid)
}

#[command]
pub fn translate_pf(msgctxt: String, msgid: String, args: Vec<String>) -> String {
    translation::_pf(&msgctxt, &msgid, &args)
}
