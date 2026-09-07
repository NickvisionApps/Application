use windows_reactor::InfoBarSeverity;

#[derive(Debug)]
pub struct InfoBarMessage {
    title: String,
    message: String,
    severity: InfoBarSeverity,
}

impl InfoBarMessage {
    pub fn new(
        title: impl Into<String>,
        message: impl Into<String>,
        severity: InfoBarSeverity,
    ) -> Self {
        InfoBarMessage {
            title: title.into(),
            message: message.into(),
            severity,
        }
    }

    pub fn title(&self) -> &str {
        &self.title
    }

    pub fn message(&self) -> &str {
        &self.message
    }

    pub fn severity(&self) -> InfoBarSeverity {
        self.severity
    }
}
