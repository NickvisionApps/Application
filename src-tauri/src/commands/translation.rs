use crate::translation::Translator;
use std::sync::Mutex;
use tauri::{State, command};

#[command]
pub fn get_available_translation_languages(
    translator: State<'_, Mutex<Translator>>,
) -> Vec<String> {
    translator.lock().unwrap().available_languages().to_vec()
}

#[command]
pub fn translate_f(
    translator: State<'_, Mutex<Translator>>,
    msgid: String,
    args: Vec<String>,
) -> String {
    translator.lock().unwrap()._f(&msgid, &args)
}

#[command]
pub fn translate_g(translator: State<'_, Mutex<Translator>>, msgid: String) -> String {
    translator.lock().unwrap()._g(&msgid)
}

#[command]
pub fn translate_n(
    translator: State<'_, Mutex<Translator>>,
    msgid: String,
    msgid_plural: String,
    n: u64,
) -> String {
    translator.lock().unwrap()._n(&msgid, &msgid_plural, n)
}

#[command]
pub fn translate_nf(
    translator: State<'_, Mutex<Translator>>,
    msgid: String,
    msgid_plural: String,
    n: u64,
    args: Vec<String>,
) -> String {
    translator
        .lock()
        .unwrap()
        ._nf(&msgid, &msgid_plural, n, &args)
}

#[command]
pub fn translate_np(
    translator: State<'_, Mutex<Translator>>,
    msgctxt: String,
    msgid: String,
    msgid_plural: String,
    n: u64,
) -> String {
    translator
        .lock()
        .unwrap()
        ._np(&msgctxt, &msgid, &msgid_plural, n)
}

#[command]
pub fn translate_npf(
    translator: State<'_, Mutex<Translator>>,
    msgctxt: String,
    msgid: String,
    msgid_plural: String,
    n: u64,
    args: Vec<String>,
) -> String {
    translator
        .lock()
        .unwrap()
        ._npf(&msgctxt, &msgid, &msgid_plural, n, &args)
}

#[command]
pub fn translate_p(
    translator: State<'_, Mutex<Translator>>,
    msgctxt: String,
    msgid: String,
) -> String {
    translator.lock().unwrap()._p(&msgctxt, &msgid)
}

#[command]
pub fn translate_pf(
    translator: State<'_, Mutex<Translator>>,
    msgctxt: String,
    msgid: String,
    args: Vec<String>,
) -> String {
    translator.lock().unwrap()._pf(&msgctxt, &msgid, &args)
}
