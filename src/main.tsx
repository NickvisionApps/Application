import "./index.css";

import React from "react";
import ReactDOM from "react-dom/client";

import {App} from "./App.tsx";

if (!import.meta.env.DEV) {
  document.addEventListener("contextmenu", (event) => {
    const target = event.target as HTMLElement;
    const isEditable =
      target.tagName === "INPUT" ||
      target.tagName === "TEXTAREA" ||
      target.isContentEditable;
    if (!isEditable) {
      event.preventDefault();
    }
  });
}

ReactDOM.createRoot(document.getElementById("root") as HTMLElement).render(
  <React.StrictMode>
    <App/>
  </React.StrictMode>,
);
