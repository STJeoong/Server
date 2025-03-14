del C:\Users\taejeong\source\repos\ServerFramework\MMOServer\DB_enum.pb.cc
del C:\Users\taejeong\source\repos\ServerFramework\MMOServer\DB_enum.pb.h
del C:\Users\taejeong\source\repos\ServerFramework\MMOServer\DB_protocol.pb.cc
del C:\Users\taejeong\source\repos\ServerFramework\MMOServer\DB_protocol.pb.h
del C:\Users\taejeong\source\repos\ServerFramework\MMOServer\DB_struct.pb.cc
del C:\Users\taejeong\source\repos\ServerFramework\MMOServer\DB_struct.pb.h
protoc -I="C:\Users\taejeong\source\repos\ServerFramework\MMOServer\Protocol" --cpp_out="C:\Users\taejeong\source\repos\ServerFramework\MMOServer" DB_protocol.proto DB_enum.proto DB_struct.proto
protoc -I="C:\Users\taejeong\source\repos\ServerFramework\MMOServer\Protocol" --csharp_out="C:\Users\taejeong\source\repos\ServerFramework_CSharp\DBServer\Protocol" DB_protocol.proto DB_enum.proto DB_struct.proto

::xcopy /y "C:\Users\taejeong\Desktop\Systems\SimulationServer\Assets\Scripts\Protocol.cs" "C:\Users\taejeong\Desktop\Systems\Unknown 2D\Assets\Scripts"

pause