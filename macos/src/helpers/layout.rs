use objc2::rc::Retained;
use objc2_app_kit::{NSLayoutConstraint, NSView};
use objc2_core_foundation::CGFloat;
use objc2_foundation::NSArray;

pub trait EasyLayout {
    fn constrain_margin(
        &self,
        parent: &NSView,
        left: Option<CGFloat>,
        top: Option<CGFloat>,
        right: Option<CGFloat>,
        bottom: Option<CGFloat>,
    );
    fn constrain_fill(&self, parent: &NSView);
    fn constrain_fill_margin(
        &self,
        parent: &NSView,
        left: CGFloat,
        top: CGFloat,
        right: CGFloat,
        bottom: CGFloat,
    );
    fn constrain_center(&self, parent: &NSView);
    fn constrain_center_horizontally(
        &self,
        parent: &NSView,
        top: Option<CGFloat>,
        bottom: Option<CGFloat>,
    );
    fn constrain_center_vertically(
        &self,
        parent: &NSView,
        left: Option<CGFloat>,
        right: Option<CGFloat>,
    );
    fn set_size(&self, width: CGFloat, height: CGFloat);
}

impl EasyLayout for NSView {
    fn constrain_margin(
        &self,
        parent: &NSView,
        left: Option<CGFloat>,
        top: Option<CGFloat>,
        right: Option<CGFloat>,
        bottom: Option<CGFloat>,
    ) {
        self.setTranslatesAutoresizingMaskIntoConstraints(false);
        let mut constraints: Vec<Retained<NSLayoutConstraint>> = vec![];
        if let Some(l) = left {
            constraints.push(
                self.leadingAnchor()
                    .constraintEqualToAnchor_constant(&parent.leadingAnchor(), l),
            )
        }
        if let Some(t) = top {
            constraints.push(
                self.topAnchor()
                    .constraintEqualToAnchor_constant(&parent.topAnchor(), t),
            )
        }
        if let Some(r) = right {
            constraints.push(
                self.trailingAnchor()
                    .constraintEqualToAnchor_constant(&parent.trailingAnchor(), -r),
            )
        }
        if let Some(b) = bottom {
            constraints.push(
                self.bottomAnchor()
                    .constraintEqualToAnchor_constant(&parent.bottomAnchor(), -b),
            )
        }
        NSLayoutConstraint::activateConstraints(&NSArray::from_retained_slice(&constraints));
    }

    fn constrain_fill(&self, parent: &NSView) {
        self.constrain_margin(parent, Some(0.0), Some(0.0), Some(0.0), Some(0.0));
    }

    fn constrain_fill_margin(
        &self,
        parent: &NSView,
        left: CGFloat,
        top: CGFloat,
        right: CGFloat,
        bottom: CGFloat,
    ) {
        self.constrain_margin(parent, Some(left), Some(top), Some(right), Some(bottom));
    }

    fn constrain_center(&self, parent: &NSView) {
        self.constrain_center_horizontally(parent, None, None);
        self.constrain_center_vertically(parent, None, None);
    }

    fn constrain_center_horizontally(
        &self,
        parent: &NSView,
        top: Option<CGFloat>,
        bottom: Option<CGFloat>,
    ) {
        self.setTranslatesAutoresizingMaskIntoConstraints(false);
        let mut constraints = vec![
            self.centerXAnchor()
                .constraintEqualToAnchor(&parent.centerXAnchor()),
        ];
        if let Some(t) = top {
            constraints.push(
                self.topAnchor()
                    .constraintEqualToAnchor_constant(&parent.topAnchor(), t),
            );
        }
        if let Some(b) = bottom {
            constraints.push(
                self.bottomAnchor()
                    .constraintEqualToAnchor_constant(&parent.bottomAnchor(), -b),
            );
        }
        NSLayoutConstraint::activateConstraints(&NSArray::from_retained_slice(&constraints));
    }

    fn constrain_center_vertically(
        &self,
        parent: &NSView,
        left: Option<CGFloat>,
        right: Option<CGFloat>,
    ) {
        self.setTranslatesAutoresizingMaskIntoConstraints(false);
        let mut constraints = vec![
            self.centerYAnchor()
                .constraintEqualToAnchor(&parent.centerYAnchor()),
        ];
        if let Some(l) = left {
            constraints.push(
                self.leadingAnchor()
                    .constraintEqualToAnchor_constant(&parent.leadingAnchor(), l),
            );
        }
        if let Some(r) = right {
            constraints.push(
                self.trailingAnchor()
                    .constraintEqualToAnchor_constant(&parent.trailingAnchor(), -r),
            );
        }
        NSLayoutConstraint::activateConstraints(&NSArray::from_retained_slice(&constraints));
    }

    fn set_size(&self, width: CGFloat, height: CGFloat) {
        self.setTranslatesAutoresizingMaskIntoConstraints(false);
        self.widthAnchor()
            .constraintEqualToConstant(width)
            .setActive(true);
        self.heightAnchor()
            .constraintEqualToConstant(height)
            .setActive(true);
    }
}
