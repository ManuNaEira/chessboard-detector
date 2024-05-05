# Chessboard Detector <!-- omit in toc -->

Repo experiment with different Chessboard corners detectors.

- [Usage](#usage)
  - [Debugging](#debugging)
- [Troubleshooting](#troubleshooting)
  - [Command "Dev Containers: Clone Repository in Container Volume... error running docker](#command-dev-containers-clone-repository-in-container-volume-error-running-docker)
  - [Not able to clone OpenCV GitHub repository from the Dev Container](#not-able-to-clone-opencv-github-repository-from-the-dev-container)

## Usage

> 📢 Repo set-up from the [CMake & OpenCV template](https://github.com/ManuNaEira/cmake_opencv).

1. Run the command (`Ctrl + Shift + P`, to open the Command Palette) **"Dev
   Containers: Clone Repository in Container Volume.."**.
2. Configure CMake, e.g. run the VSCode command **CMake: Configure**, select the
   compiler to be used, GCC has the one being tested so far.
3. Build the target `executable` (defined in the `app/` directory), e.g. run the
   VSCode command **CMake: Build Target** and select the `executable` one.
4. Run the built file, i.e. it is located in `build/bin/executable`. It should
   produce the image `detected_corners.jpg` with the detected corners overlaid
   on top of the input (gray scaled) image.

### Debugging

1. Set a breakpoint in the `app/main.cpp` file.
2. At the bottom of VSCode, in the Status Bar, select executable as the default
   target and click the debug button.
3. The program execution should stop at the breakpoint previously set. BONUS:
   using the extension [OpenCV C++
   Image](https://marketplace.visualstudio.com/items?itemName=SimpleToolsDev.opencv-image)
   we can easily visualize cv::Mat contents.

## Troubleshooting

### Command "Dev Containers: Clone Repository in Container Volume... error running docker

Here we are suffering the registered issue:
https://github.com/microsoft/vscode-remote-release/issues/6817

The solution is the one provided there as well, i.e. ensure the setting
`"dev.containers.executeInWSL": true` in VSCode.

### Not able to clone OpenCV GitHub repository from the Dev Container

This could likely be due to do not share properly the SSH credentials, which
happened to me when creating the Dev Container from WSL.

Issue was fixed by adding the following lines at the end of the `~/.bashrc` file
in order to automatically start the ssh-agent and add the relevant keys every
time a Terminal session is started: 

```bash
ssh-add &>/dev/null || eval "$(ssh-agent -s)" &>/dev/null   # start ssh-agent if not present
[ $? -eq 0 ] && {                                           # ssh-agent has started
ssh-add $HOME/.ssh/id_ed25519                               # Example private key 1
ssh-add $HOME/.ssh/id_ed25519_github                        # Example private key 2
}
```
