use semver::Version;
use std::collections::HashMap;

pub struct AppInfo {
    id: &'static str,
    name: &'static str,
    description: &'static str,
    english_short_name: &'static str,
    short_name: &'static str,
    version: Version,
    changelog: &'static str,
    is_portable: bool,
    repo_url: &'static str,
    issues_url: &'static str,
    discussion_url: &'static str,
    developers: HashMap<&'static str, &'static str>,
    designers: HashMap<&'static str, &'static str>,
    artists: HashMap<&'static str, &'static str>,
}

impl AppInfo {
    pub fn id(&self) -> &str {
        self.id
    }

    pub fn name(&self) -> &str {
        self.name
    }

    pub fn description(&self) -> &str {
        self.description
    }

    pub fn english_short_name(&self) -> &str {
        self.english_short_name
    }

    pub fn short_name(&self) -> &str {
        self.short_name
    }

    pub fn version(&self) -> &Version {
        &self.version
    }

    pub fn changelog(&self) -> &str {
        self.changelog
    }

    pub fn is_portable(&self) -> bool {
        self.is_portable
    }

    pub fn repo_url(&self) -> &str {
        self.repo_url
    }

    pub fn issues_url(&self) -> &str {
        self.issues_url
    }

    pub fn discussion_url(&self) -> &str {
        self.discussion_url
    }

    pub fn developers(&self) -> &HashMap<&str, &str> {
        &self.developers
    }

    pub fn designers(&self) -> &HashMap<&str, &str> {
        &self.designers
    }

    pub fn artists(&self) -> &HashMap<&str, &str> {
        &self.artists
    }
}

impl Default for AppInfo {
    fn default() -> Self {
        Self {
            id: "org.nickvision.application",
            name: "Nickvision Application",
            description: "Create new Nickvision applications",
            english_short_name: "Application",
            short_name: "Application",
            version: Version::parse(format!("{}-next", env!("CARGO_PKG_VERSION")).as_str())
                .unwrap(),
            changelog: "- Initial release",
            is_portable: std::env::args().any(|arg| arg == "--portable"),
            repo_url: "https://github.com/NickvisionApps/Application",
            issues_url: "https://github.com/NickvisionApps/Application/issues",
            discussion_url: "https://github.com/NickvisionApps/Application/discussions",
            developers: HashMap::from([
                ("Nicholas Logozzo", "https://github.com/nlogozzo"),
                (
                    "Contributors on GitHub ❤️",
                    "https://github.com/NickvisionApps/Application/graphs/contributors",
                ),
            ]),
            designers: HashMap::from([
                ("Nicholas Logozzo", "https://github.com/nlogozzo"),
                ("Fyodor Sobolev", "https://github.com/fysobolev"),
                ("DaPigGuy", "https://github.com/DaPigGuy"),
            ]),
            artists: HashMap::from([("David Lapshin", "https://github.com/daudix")]),
        }
    }
}
