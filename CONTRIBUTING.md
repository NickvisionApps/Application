# Contributing to Nickvision Application

First off, thanks for taking the time to contribute! ❤️

All types of contributions are encouraged and valued. See the [Table of Contents](#table-of-contents) for different ways
to help and details about how this project handles them. Please make sure to read the relevant section before making
your contribution. It will make it a lot easier for us maintainers and smooth out the experience for all involved. The
community looks forward to your contributions. 🎉

> And if you like the project, but just don't have time to contribute, that's fine. There are other easy ways to support
> the project and show your appreciation, which we would also be very happy about:
>
> - Star the project
> - Post about it
> - Reference this project in your project's readme
> - [Sponsor](https://github.com/sponsors/nlogozzo) the lead developer

## Table of Contents

- [I Have a Question](#i-have-a-question)
- [I Want To Contribute](#i-want-to-contribute)
  - [Reporting Bugs](#reporting-bugs)
  - [Suggesting Enhancements/New Features](#suggesting-enhancements)
  - [Providing translations](#providing-translations)
    - [Via Weblate](#via-weblate)
    - [Manually](#manually)
  - [Your First Code Contribution](#your-first-code-contribution)
- [Styleguides](#styleguides)
- [Join The Project Team](#join-the-project-team)

## I Have a Question

Before you ask a question, it is best to search for
existing [Discussions](https://github.com/NickvisionApps/Application/discussions)
and [Issues](https://github.com/NickvisionApps/Application/issues) that might help you.

In case you have found a suitable existing issue/discussion and still need clarification, you can write your question in
said post. It is also advisable to search the internet for answers first to common error messages.

If you then still feel the need to ask a question and need clarification, we recommend the following:

- Open a [Discussion](https://github.com/NickvisionApps/Application/discussions).
- Provide as much context as you can about what you're running into.
- Provide project and platform versions (windows, gnome, etc...), depending on what seems relevant.

We will then take care of the question as soon as possible and convert it to a proper issue, if needed.

## I Want To Contribute

### Legal Notice

When contributing to this project, you must agree that you have authored 100% of the content and/or that you have the
necessary rights to the content and that the content you contribute may be provided under the
project [license](LICENSE).

### Reporting Bugs

#### Before Submitting a Bug Report

A good bug report shouldn't leave others needing to chase you up for more information. Therefore, we ask that you
investigate carefully, collect necessary information and describe the issue in detail in your report. Please complete
the following steps in advanced to help us fix any potential bug as fast as possible:

- Make sure that you are using the latest released stable version.
- Determine if your bug is really a bug and not an error on your side. If you are looking for support, you might want to
  check [this section](#i-have-a-question).
- See if other users have experienced (and potentially already solved) the same issue you are having, check if there is
  not already a bug report existing for your bug or error in both
  the [Discussions](https://github.com/NickvisionApps/Application/discussions)
  and [Issues](https://github.com/NickvisionApps/Application/issues) sections.
- Collect information about the bug:
  - Debug information provided by the application
    - From the user menu at the bottom of the sidebar, open Help → Debugging Information and copy the information to
      the clipboard to paste in your issue.
  - Stack trace (Traceback)
    - Including any error messages thrown by the application
    - You may need to start the application via the terminal/console to receive an error message for a crash.
  - OS, Platform and Version (Distro, Kernel Version, x64/ARM64, etc...)
  - Your input and the output to the application
    - i.e. Steps you took to produce the crash and/or attach any files you may have opened within the app that
      caused a crash
  - Can you reliably reproduce the issue? And can you also reproduce it with older versions?

#### How Do I Submit a Good Bug Report?

You must never report security related issues, vulnerabilities and bugs (including sensitive information) to the issue
tracker nor elsewhere in public. Instead, sensitive issues must be reported and handled via email
to <nlogozzo225@gmail.com>.

We use GitHub issues to track bugs and errors. If you run into an issue with the project:

- Open a [new Issue](https://github.com/NickvisionApps/Application/issues/new) and explain the behavior you are
  experiencing and what you expect to happen.
- Please provide as much context as possible and describe the _reproduction steps_ that someone else can follow to
  recreate the issue on their own system. For good bug reports you should isolate the problem and create a reduced test
  case.
- Provide the information you collected in the previous section.

Once it has been opened:

- The project team will label the issue accordingly.
- A team member will try to reproduce the issue with your provided steps. If there are no reproduction steps or no
  obvious way to reproduce the issue, the team will ask you for those steps.
  - Bugs that are not able to be reproduced will not be addressed until they are reproduced. Therefore, it is
    important to include steps to speed up the fixing process.

### Suggesting Enhancements

This section guides you through submitting an enhancement suggestion for Nickvision Application, **including completely
new features and minor improvements to existing functionality**. Following these guidelines will help maintainers and
the community in understanding your suggestion and finding related suggestions.

#### Before Submitting an Enhancement

- Make sure that you are using the latest released version.
- Perform a search through [Discussions](https://github.com/NickvisionApps/Application/discussions)
  and [Issues](https://github.com/NickvisionApps/Application/issues) to see if the enhancement has already been
  suggested. If it has, add a comment to the existing issue instead of opening a new one.
- Find out whether your idea fits with the scope and aims of the project. It's up to you to make a strong case to
  convince the project's developers of the merits of this feature. Keep in mind that we want features that will be
  useful to the majority of our users and not just a small subset.

#### How Do I Submit a Good Enhancement Suggestion?

Enhancement suggestions are tracked as [GitHub issues](https://github.com/NickvisionApps/Application/issues).

- Use a **clear and descriptive title** for the issue to identify the suggestion.
- Provide a **step-by-step description of the suggested enhancement** in as many details as possible.
  - For enhancements to existing functionality, **describe the current behavior** and **explain which behavior you
    expected to see instead** and why. At this point you can also tell which alternatives do not work for you.
  - For completely new features, **describe what you would like to see from this new feature** in terms of **both
    functionality and design**. Providing mockups, even if in sketch format, greatly help the team envision what you
    would like to see.
- **Explain why this enhancement would be useful** to most Application users. You may also want to point out similar
  projects of other platforms and their solutions to serve as inspiration.

### Providing Translations

Everyone is welcome to translate this app into their native or known languages, so that the application is accessible to
everyone.

##### Via Weblate

Application is available to translate on [Weblate](https://hosted.weblate.org/engage/nickvision-application/)!

##### Manually

To start translating the app, fork the repository and clone it locally.

Application uses [gettext](https://www.gnu.org/software/gettext/manual/gettext.html#PO-Files) for translations. In the
`resources/po` folder you will find files that can be edited in your favourite `*.po` files editor (or with any plain
text editor).

If you want to create a new translation, copy the `application.pot` file and rename said copy as `<lang_code>.po`, where
`<lang_code>` is the language code for your translation. Usually the code is two letters, but it can also be a specific
locale code to differentiate between versions of the same language (for example, `pt` and `pt_BR`).

**Also, add the language code to `LINGUAS` file** (keeping this file in alphabetical order).

Edit your new translation file with correct translations for the English messages.

To check your translation file, make sure your system is in the locale of the language you are translating
and [locally build and run the app](README.md#building) using `npm run tauri dev`. If all steps were carried out
successfully, you should see your translation in action!

Once all changes to your translated file are made, commit these changes and create a pull request to the project.

### Your First Code Contribution

#### Structure

Application is built using [Tauri](https://tauri.app), pairing a Rust backend with a React/TypeScript frontend. With
this stack, Application is built for the Windows, macOS, and Linux operating systems from a single shared codebase.

The project is split up into the following top-level folders:

- [src-tauri](#src-tauri)
- [src](#src)

##### src-tauri

This folder contains the Rust backend of the app:

- `src/commands` => Thin [Tauri command](https://tauri.app/develop/calling-rust/) handlers exposed to the frontend over
  IPC.
- `src/controller.rs`, `config.rs`, `folder.rs`, `product`, `translation.rs` => The business logic and data models of
  the application.
- `capabilities` => Tauri permission manifests scoping what the frontend is allowed to call.

##### src

This folder contains the React/TypeScript frontend of the app:

- `components` => The UI of the app, including `dialogs` (modal dialogs), `layout` (layout primitives), `pages`
  (top-level views), and `ui` (generated [shadcn/ui](https://ui.shadcn.com/) primitives, which should not be
  hand-edited).
- `hooks` => Reusable React hooks.
- `lib` => React context providers that expose app state and Tauri commands to components (configuration, translation,
  dialogs, navigation, etc...).

## Styleguides

Application uses:

- [rustfmt](https://github.com/rust-lang/rustfmt) conventions for Rust code (`src-tauri`)
- [ESLint](https://eslint.org/) (`npm run lint`) and [Prettier](https://prettier.io/) (`npm run format`) for
  TypeScript/React code (`src`)

Please run `npm run lint` and `npm run format` before submitting a pull request. Generated code (`src/components/ui`) is
excluded from linting/formatting and should not be manually restyled.

## Join The Project Team

<a href='https://matrix.to/#/#nickvision:matrix.org'><img width='140' alt='Join our room' src='https://user-images.githubusercontent.com/17648453/196094077-c896527d-af6d-4b43-a5d8-e34a00ffd8f6.png'/></a>

## Attribution

This guide was based on a template by [contributing-gen](https://github.com/bttger/contributing-gen).
