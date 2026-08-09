-- Cobalt2D platformer sample. Attach this script to the Player object.
function onStart()
  print("Player started")
end

function onUpdate(dt)
  local speed = 180
  if Input.isDown("MoveLeft") then
    transform.position.x = transform.position.x - speed * dt
  end
  if Input.isDown("MoveRight") then
    transform.position.x = transform.position.x + speed * dt
  end
  if Input.isPressed("Jump") then
    rigidbody:applyImpulse(0, 500)
  end
end

function onFixedUpdate(dt)
  -- Physics-driven movement belongs here when fixed-step gameplay is enabled.
end

function onCollision(other)
  print("Collision with " .. other.name)
end