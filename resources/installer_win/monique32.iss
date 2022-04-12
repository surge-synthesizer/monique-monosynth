#define MyAppPublisher "Surge Synth Team"
#define MyAppURL "https://www.surge-synth-team.org/"
#define MyAppName "MoniqueMonosynth"
#define MyAppNameCondensed "Monique"
#define MyID "8D0A95D4-10AB-4457-9366-9A86CF7A7B8G"

;uncomment these two lines if building the installer locally!
;#define MONIQUE_SRC "..\..\"
;#define MONIQUE_BIN "..\..\build\"

[Setup]
AppId={#MyID}
AppName={#MyAppName}
AppVersion={#MONIQUE_VERSION}
AppVerName={#MyAppName} {#MONIQUE_VERSION}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppPublisher}\{#MyAppName}\
DefaultGroupName={#MyAppName}
DisableDirPage=yes
DisableProgramGroupPage=yes
AlwaysShowDirOnReadyPage=yes
LicenseFile={#MONIQUE_SRC}\LICENSE-gpl3
OutputBaseFilename={#MyAppName}-win32-{#MONIQUE_VERSION}-setup
SetupIconFile={#MONIQUE_SRC}\resources\installer_win\monique.ico
UninstallDisplayIcon={uninstallexe}
UsePreviousAppDir=yes
Compression=lzma
SolidCompression=yes
UninstallFilesDir={autoappdata}\{#MyAppName}\uninstall
CloseApplicationsFilter=*.exe,*.vst3
WizardStyle=modern
WizardSizePercent=100
WizardImageFile={#MONIQUE_SRC}\resources\installer_win\empty.bmp
WizardSmallImageFile={#MONIQUE_SRC}\resources\installer_win\empty.bmp
WizardImageAlphaFormat=defined

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Types]
Name: "full"; Description: "Full installation"
Name: "plugin"; Description: "VST3 installation"
Name: "standalone"; Description: "Standalone installation"
Name: "custom"; Description: "Custom"; Flags: iscustom

[Components]
Name: "vst3"; Description: "{#MyAppNameCondensed} VST3 (32-bit)"; Types: full plugin custom
Name: "exe"; Description: "{#MyAppNameCondensed} Standalone (32-bit)"; Types: full standalone custom
Name: "docs"; Description: "{#MyAppNameCondensed} Manual"; Types: full plugin standalone custom

[Files]
Source: "{#MONIQUE_BIN}\monique_products\{#MyAppName}.exe"; DestDir: "{app}"; Components: exe; Flags: ignoreversion
Source: "{#MONIQUE_BIN}\monique_products\{#MyAppName}.vst3\*"; DestDir: "{autocf}\VST3\{#MyAppName}.vst3\"; Components: vst3; Flags: ignoreversion recursesubdirs
Source: "{#MONIQUE_SRC}\resources\manual\Manual-English.pdf"; DestDir: "{app}"; Components: docs; Flags: ignoreversion

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppName}.exe"; Flags: createonlyiffileexists
