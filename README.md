# DeathBall
DeathBall - Sadistic football using OpenGL

![image info](./uml/Sketch.png)

### UML Diagram

![image info](./uml/DeathBall-ClassDiagram.png)


```plantuml
@startuml

hide empty methods
hide empty attributes

title DeathBall Game - Class Diagram

interface IGameObject {
    +void Update()
    +void Input()
    +void Draw()
}

interface IObjectShape {
    +void Draw()
}

class DeathFootBallPlayerShape {
    +void Draw()
}
class WallShape {
    +void Draw()
}
class BallShape {
    +void Draw()
}

class ObjectCreator {
    + IGameObject* CreateObject()
}

class DeathFootBallPlayer {
}

class Board {

}
class Wall {
}

class Ball {

}

interface IControl {

}
class Engine {
    +void Init()
    +void Input()
    +void Update()
    +void Draw()
}

IObjectShape <|-- DeathFootBallPlayerShape
IObjectShape <|-- WallShape
IObjectShape <|-- BallShape

DeathFootBallPlayer --> IObjectShape
Ball --> IObjectShape
Wall --> IObjectShape
Board --> IObjectShape
Board --> Wall

IGameObject <|-- DeathFootBallPlayer
IGameObject <|-- Ball
IGameObject <|-- Wall
IGameObject <|-- Board

ObjectCreator --> IGameObject
ObjectCreator --> Engine
Engine --> IGameObject
Engine -left-> IControl

@enduml
```

### Materials
* [OpenGL Tutorial ](https://learnopengl.com/Introduction/)
* [Plant Uml Generator ](https://www.planttext.com)
* [Plant Uml Doc](https://plantuml.com/class-diagram)



