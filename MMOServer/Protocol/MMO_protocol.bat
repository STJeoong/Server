del C:\Users\taejeong\source\repos\ServerFramework\MMOServer\*.pb.cc
del C:\Users\taejeong\source\repos\ServerFramework\MMOServer\*.pb.h
protoc -I="C:\Users\taejeong\source\repos\ServerFramework\MMOServer\Protocol" --cpp_out="C:\Users\taejeong\source\repos\ServerFramework\MMOServer" MMO_protocol.proto MMO_enum.proto MMO_struct.proto
protoc -I="C:\Users\taejeong\source\repos\ServerFramework\MMOServer\Protocol" --csharp_out="C:\Users\taejeong\Desktop\Systems\Unknown 2D\Assets\@Scripts\Protocol" MMO_protocol.proto MMO_enum.proto MMO_struct.proto

::xcopy /y "C:\Users\taejeong\Desktop\Systems\SimulationServer\Assets\Scripts\Protocol.cs" "C:\Users\taejeong\Desktop\Systems\Unknown 2D\Assets\Scripts"

pause