<div align="center">
  <picture>
    <source media="(prefers-color-scheme: light)" srcset="https://github.com/mozahzah/IECore/raw/master/Resources/IE-Brand-Kit/IE-Logo-Alt-NoBg.png">
    <source media="(prefers-color-scheme: dark)" srcset="https://github.com/mozahzah/IECore/raw/master/Resources/IE-Brand-Kit/IE-Logo-NoBg.png">
  <img alt="IELogo" src="https://github.com/mozahzah/IECore/raw/master/Resources/IE-Brand-Kit/IE-Logo-NoBg.png" width="256">
  </picture>
</div>

# IECore

**IECore** is a open-source library designed for Windows and macOS applications that leverages ImGui as its user interface.
The library includes an abstracted renderer class, robust logging and assertion features, as well as useful utilities and ImGui extension logic.  
Currently, only Vulkan is implemented as a rendering backend, but support for additional backends will be added in the future.  

> [!IMPORTANT] 
> Find the IECore wiki here: [IECore Wiki](https://github.com/mozahzah/IECore/wiki)

## Features
- **Abstracted Renderer Class**: Provides a unified interface for different rendering backends.
- **Robust Logging**: Offers detailed logging capabilities for debugging and monitoring.
- **Utilities**: Provides various helpful tools and functions for application development.
- **ImGui Extension Logic**: Enhances ImGui with additional functionality and customizations.

## ImGui Extensions
The ImGui namespace is extended with several useful functions to enhance the flexibility and ease of use within the ImGui framework.  
These extensions provide enhanced cursor positioning and additional useful widgets.  
### Highlighted features
- **SmartCursorPositioning**: Sets the cursor position relative to the current window size by providing a scalar between 0 - 1.
- **CenteredText(const char\* Fmt, ...)**: Renders text centered horizontally within the current window.
- **FileFinder(const char\* Label, int Depth, std::string& SelectedFile)**: Opens a file finder dialog that allows users to browse directories and select files. The file finder widget will automatically reset once the directory depth specified by the Depth parameter is reached. The function stores the path of the selected file in the SelectedFile parameter.

## Third-Party Libraries Used
- [Dear ImGui](https://github.com/ocornut/imgui)
- [stb](https://github.com/nothings/stb)

## Contribution
Contributors are welcomed to this open-source project. Any feedback or assistance, whether in coding, packaging, documentation, design, or testing, is greatly appreciated. 

## License
This work is licensed under the [SPDX-License-Identifier: GPL-2.0-only](./LICENSE).
