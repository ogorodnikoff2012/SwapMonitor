# Swap Monitor

Swap Monitor is a lightweight system tray application that monitors swap memory usage and provides visual notifications when swap is in use.

## Features
- Displays a tray icon to indicate swap activity.
- Reduces or eliminates the use of swap memory to optimize system performance.
- Simple and easy-to-use interface.

---

## Requirements

Ensure the following dependencies are installed on your system:
- GCC (or any C compiler)
- CMake (version 3.10 or higher)
- GTK+ 3.0 development files
- libappindicator3 development files

To install dependencies on Ubuntu:
```bash
sudo apt update
sudo apt install build-essential cmake libgtk-3-dev libappindicator3-dev
```

---

## Build Instructions

1. Clone the repository:
   ```bash
   git clone https://github.com/ogorodnikoff2012/SwapMonitor.git
   cd SwapMonitor
   ```

2. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```

3. Configure the project:
   ```bash
   cmake ..
   ```

4. Build the application:
   ```bash
   make
   ```

---

## Installation Instructions

1. Install the application (requires root privileges):
   ```bash
   sudo make install
   ```

2. This will:
   - Install the binary to `/usr/local/bin/swap_tray`.
   - Add a `.desktop` file to `/usr/share/applications/swap_monitor.desktop`.
   - Install the icon to `/usr/share/icons/hicolor/128x128/apps/swap_monitor.png`.

---

## Autostart

To enable autostart for the Swap Monitor application:

1. Copy the `.desktop` file to your autostart directory:
   ```bash
   mkdir -p ~/.config/autostart
   cp /usr/share/applications/swap_monitor.desktop ~/.config/autostart/
   ```

2. Restart your system or re-login to ensure the application starts automatically.

---

## Usage

1. Run the application manually (if not in autostart):
   ```bash
   /usr/local/bin/swap_tray
   ```

2. A tray icon will appear to indicate the current swap usage status:
   - **Green**: No swap is in use.
   - **Red (animated)**: Swap is active.

3. Right-click the tray icon to open the menu and quit the application.

---

## Uninstallation

To remove the application:

1. Delete installed files:
   ```bash
   sudo rm /usr/local/bin/swap_tray
   sudo rm /usr/share/applications/swap_monitor.desktop
   sudo rm /usr/share/icons/hicolor/128x128/apps/swap_monitor.png
   ```

2. If you enabled autostart, remove the file from the autostart directory:
   ```bash
   rm ~/.config/autostart/swap_monitor.desktop
   ```

---

## Contributing

Feel free to submit issues or pull requests to improve the application.

---

## License

This project is licensed under the MIT License. See the LICENSE file for details.

