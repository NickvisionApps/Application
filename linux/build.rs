fn main() {
    glib_build_tools::compile_resources(
        &["../resources/linux"],
        "../resources/linux/resources.gresource.xml",
        "compiled.gresource",
    );
}
