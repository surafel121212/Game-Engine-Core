# GitHub Actions

The included workflow is intentionally debug-first. It installs its own Java,
Android SDK, NDK, CMake, and Gradle versions on every runner, so a clean
checkout does not depend on a developer's machine.

The resulting artifact is `cobalt2d-debug-apk`. To add a release build:

1. Create a keystore outside the repository.
2. Store its values in GitHub encrypted secrets.
3. Add a `signingConfigs.release` block to `android/app/build.gradle`.
4. Add `assembleRelease` and upload the release output.

Never commit `keystore.properties`, a `.jks` file, or signing passwords.