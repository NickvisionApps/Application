use crate::product::ProductInfo;
use semver::Version;
use serde::{Deserialize, Serialize};
use tauri::AppHandle;
use tauri_plugin_updater::{Update, UpdaterExt};
use url::Url;

#[derive(Debug, Clone, PartialEq, Eq, Serialize, Deserialize)]
#[serde(rename_all = "camelCase")]
pub struct UpdateDownloadProgress {
    downloaded: usize,
    total: u64,
}

impl UpdateDownloadProgress {
    pub fn new(downloaded: usize, total: u64) -> Self {
        Self { downloaded, total }
    }

    pub fn downloaded(&self) -> usize {
        self.downloaded
    }

    pub fn total(&self) -> u64 {
        self.total
    }
}

pub async fn latest_update(
    app: &AppHandle,
    product_info: &ProductInfo,
    allow_preview: bool,
) -> Option<Update> {
    let update = app
        .updater_builder()
        .endpoints(vec![
            if allow_preview {
                format!(
                    "https://github.com/{}/{}/releases/download/preview/latest.json",
                    product_info.repo_owner(),
                    product_info.repo_name()
                )
            } else {
                format!(
                    "https://github.com/{}/{}/releases/latest/download/latest.json",
                    product_info.repo_owner(),
                    product_info.repo_name()
                )
            }
            .parse::<Url>()
            .unwrap(),
        ])
        .unwrap()
        .build()
        .unwrap()
        .check()
        .await
        .ok()
        .flatten();
    if let Some(update) = update {
        let version = Version::parse(&update.version).ok()?;
        if &version > product_info.version() {
            return Some(update);
        }
    }
    None
}
