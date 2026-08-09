Fix the GitHub Actions Android build failure.

The GitHub Actions build currently fails at:

Cobalt2D/android/gradlew

with:

./gradlew: 55: Syntax error: "(" unexpected
Error: Process completed with exit code 2.

This is a Gradle wrapper script problem, not an Android SDK installation problem.

Inspect the existing Gradle wrapper files:

Cobalt2D/android/gradlew
Cobalt2D/android/gradle/wrapper/gradle-wrapper.jar
Cobalt2D/android/gradle/wrapper/gradle-wrapper.properties

Fix the Gradle wrapper correctly.

Do NOT simply hide or ignore the error.
Do NOT bypass Gradle.
Do NOT replace the Android build with a fake APK.

Make sure Cobalt2D/android/gradlew is a valid standard Unix Gradle wrapper script with:

- correct shebang
- valid POSIX shell syntax
- Unix LF line endings
- executable permission
- no BOM
- no corrupted characters
- correct Gradle wrapper configuration

If the existing gradlew is corrupted, regenerate/replace it with a proper Gradle wrapper compatible with the project's Android Gradle Plugin and Gradle version.

Also inspect gradle-wrapper.properties and make sure the distributionUrl uses a Gradle version compatible with the Android Gradle Plugin used by this project.

Do not blindly force Gradle 9.3.1 if the Android Gradle Plugin does not support it.

Then inspect the GitHub Actions workflow.

The workflow currently contains:

gradle/actions/setup-gradle@v4
gradle-version: "9.3.1"

and then executes:

cd Cobalt2D/android && ./gradlew --no-daemon assembleDebug

Make the workflow use the project's Gradle wrapper correctly.

After fixing it:

1. Run the Gradle wrapper.
2. Run assembleDebug.
3. Fix any subsequent Android/CMake/NDK/C++ errors.
4. Verify that:
   Cobalt2D/android/app/build/outputs/apk/debug/app-debug.apk
   is actually generated.
5. Make sure GitHub Actions uploads the APK artifact.

Do not stop after fixing the first error. Continue fixing the build until the Android debug APK can actually be generated.

Do not rebuild the game engine from scratch.
Do not delete existing engine functionality.
Only fix what is necessary for the Android build.
