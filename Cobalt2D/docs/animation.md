# Animation

`Animator` stores named `AnimationClip` values. Each clip has ordered texture
frames, per-frame durations, loop state, and playback speed. Call `play`,
`pause`, `resume`, `stop`, and `update`. The current frame is available through
`currentFrame()` for a sprite renderer.