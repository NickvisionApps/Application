import {ReactNode} from "react";

interface TitlebarControlProps {
  children?: ReactNode;
}

export function TitlebarControl({children}: TitlebarControlProps) {
  return (
    <div className="titlebar-drag-region" data-tauri-drag-region>
      {children}
    </div>
  );
}
