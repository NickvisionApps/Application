use semver::Version;
use serde::{Deserialize, Serialize};
use serde_with::{DisplayFromStr, serde_as};
use std::fmt::{Display, Formatter};

#[derive(Debug, Clone, Eq, PartialEq)]
pub enum DeploymentMode {
    Local,
    Flatpak,
    Snap,
    Wsl,
}

#[serde_as]
#[derive(Debug, Clone, PartialEq, Eq, Serialize, Deserialize)]
#[serde(rename_all = "camelCase")]
pub struct ProductInfo {
    id: String,
    name: String,
    short_name: String,
    repo_owner: String,
    repo_name: String,
    description: String,
    #[serde_as(as = "DisplayFromStr")]
    version: Version,
}

#[derive(Default)]
pub struct ProductInfoBuilder {
    id: Option<String>,
    name: Option<String>,
    short_name: Option<String>,
    repo_owner: Option<String>,
    repo_name: Option<String>,
    description: Option<String>,
    version: Option<Version>,
}

impl ProductInfo {
    pub fn builder() -> ProductInfoBuilder {
        ProductInfoBuilder::default()
    }

    pub fn id(&self) -> &str {
        &self.id
    }

    pub fn name(&self) -> &str {
        &self.name
    }

    pub fn short_name(&self) -> &str {
        &self.short_name
    }

    pub fn repo_owner(&self) -> &str {
        &self.repo_owner
    }

    pub fn set_repo_owner(&mut self, repo_owner: impl Into<String>) {
        self.repo_owner = repo_owner.into();
    }

    pub fn repo_name(&self) -> &str {
        &self.repo_name
    }

    pub fn set_repo_name(&mut self, repo_name: impl Into<String>) {
        self.repo_name = repo_name.into();
    }

    pub fn description(&self) -> &str {
        &self.description
    }

    pub fn set_description(&mut self, description: impl Into<String>) {
        self.description = description.into();
    }

    pub fn version(&self) -> &Version {
        &self.version
    }

    pub fn repo_url(&self) -> String {
        format!("https://github.com/{}/{}", self.repo_owner, self.repo_name)
    }

    pub fn issues_url(&self) -> String {
        format!(
            "https://github.com/{}/{}/issues",
            self.repo_owner, self.repo_name
        )
    }

    pub fn discussions_url(&self) -> String {
        format!(
            "https://github.com/{}/{}/discussions",
            self.repo_owner, self.repo_name
        )
    }

    pub fn deployment_mode() -> DeploymentMode {
        #[cfg(any(target_os = "windows", target_os = "macos"))]
        return DeploymentMode::Local;
        #[cfg(target_os = "linux")]
        {
            if let Ok(res) = std::fs::exists("./flatpak-info")
                && res
            {
                DeploymentMode::Flatpak
            } else if std::env::var("SNAP").is_ok() {
                DeploymentMode::Snap
            } else if let Ok(res) = std::fs::exists("/proc/sys/fs/binfmt_misc/WSLInterop")
                && res
            {
                DeploymentMode::Wsl
            } else {
                DeploymentMode::Local
            }
        }
    }

    pub fn is_portable() -> bool {
        std::env::args().any(|arg| arg == "--portable")
    }
}

impl ProductInfoBuilder {
    pub fn id(mut self, id: impl Into<String>) -> Self {
        self.id = Some(id.into());
        self
    }

    pub fn name(mut self, name: impl Into<String>) -> Self {
        self.name = Some(name.into());
        self
    }

    pub fn short_name(mut self, short_name: impl Into<String>) -> Self {
        self.short_name = Some(short_name.into());
        self
    }

    pub fn repo_owner(mut self, repo_owner: impl Into<String>) -> Self {
        self.repo_owner = Some(repo_owner.into());
        self
    }

    pub fn repo_name(mut self, repo_name: impl Into<String>) -> Self {
        self.repo_name = Some(repo_name.into());
        self
    }

    pub fn description(mut self, description: impl Into<String>) -> Self {
        self.description = Some(description.into());
        self
    }

    pub fn version(mut self, version: Version) -> Self {
        self.version = Some(version);
        self
    }

    pub fn build(self) -> Result<ProductInfo, &'static str> {
        Ok(ProductInfo {
            id: self.id.ok_or("id is required")?,
            name: self.name.ok_or("name is required")?,
            short_name: self.short_name.ok_or("short_name is required")?,
            repo_owner: self.repo_owner.unwrap_or_default(),
            repo_name: self.repo_name.unwrap_or_default(),
            description: self.description.unwrap_or_default(),
            version: self.version.ok_or("version is required")?,
        })
    }
}

impl Display for DeploymentMode {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(
            f,
            "{}",
            match self {
                DeploymentMode::Local => "Local",
                DeploymentMode::Flatpak => "Flatpak",
                DeploymentMode::Snap => "Snap",
                DeploymentMode::Wsl => "WSL",
            }
        )
    }
}
