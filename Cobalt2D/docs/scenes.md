# Scenes and entities

Create entities with `Scene::createObject`, attach typed components with
`Entity::add<T>()`, query with `get<T>()`, and remove with `remove<T>()`.
Parenting is represented by an entity ID so hierarchy data survives
serialization.

Save with `SceneSerializer::save` and load with `SceneSerializer::load`.
Scene files are intended to be diffable and reviewable in Git.