fn main() {
    glib_build_tools::compile_resources(
        &["../resources/linux"],
        &format!("../resources/linux/{}.gresource.xml", ID),
        &format!("{}.gresource", ID),
    );
}