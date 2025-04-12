[Setup]
AppName=Универсальный Калькулятор
AppVersion=1.0
DefaultDirName={pf}\Универсальный Калькулятор
DefaultGroupName=Универсальный Калькулятор
OutputDir=.
OutputBaseFilename=Setup

[Files]
Source: "C:\Users\ctpok\OneDrive\Рабочий стол\Уник\3 курс\6 семестр\ТРПО\TRPO4\a.exe"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\Моё Приложение"; Filename: "{app}\a.exe"

[Run]
Filename: "{app}\a.exe"; Description: "Запустить после установки"; Flags: postinstall nowait skipifsilent