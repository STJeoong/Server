protoc -I="C:\Users\taejeong\source\repos\ServerFramework\GameServer\Protocol" --cpp_out="C:\Users\taejeong\source\repos\ServerFramework\GameServer" game_protocol.proto
protoc -I="C:\Users\taejeong\source\repos\ServerFramework\GameServer\Protocol" --csharp_out="C:\Users\taejeong\Desktop\Systems\Unknown 2D\Assets\Scripts\Protocol" game_protocol.proto

::xcopy /y "C:\Users\taejeong\Desktop\Systems\SimulationServer\Assets\Scripts\Protocol.cs" "C:\Users\taejeong\Desktop\Systems\Unknown 2D\Assets\Scripts"

pause