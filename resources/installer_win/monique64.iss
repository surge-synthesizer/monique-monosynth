#define MyAppPublisher "Surge Synth Team"
#define MyAppURL "https://www.surge-synth-team.org/"
#define MyAppName "MoniqueMonosynth"
#define MyAppNameCondensed "Monique"
#define MyID "8D0A95D4-10AB-4457-9366-9A86CF7A7B8F"

;uncomment these two lines if building the installer locally!
;#define MONIQUE_SRC "..\..\"
;#define MONIQUE_BIN "..\..\build\"

[Setup]
ArchitecturesInstallIn64BitMode=x64
ArchitecturesAllowed=x64
AppId={#MyID}
AppName={#MyAppName}
AppVerName={#MyAppName}
AppVersion={#MONIQUE_VERSION}
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
OutputBaseFilename={#MyAppName}-{#MONIQUE_VERSION}-Windows-64bit-setup
SetupIconFile={#MONIQUE_SRC}\resources\installer_win\monique.ico
UninstallDisplayIcon={uninstallexe}
UsePreviousAppDir=yes
Compression=lzma
SolidCompression=yes
UninstallFilesDir={app}\uninstall
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
Name: "vst3"; Description: "VST3 installation"
Name: "clap"; Description: "CLAP installation"
Name: "standalone"; Description: "Standalone installation"
Name: "custom"; Description: "Custom"; Flags: iscustom

[Components]
Name: "VST3"; Description: "{#MyAppName} VST3 (64-bit)"; Types: full vst3 custom
Name: "CLAP"; Description: "{#MyAppName} CLAP (64-bit)"; Types: full clap custom
Name: "SA"; Description: "{#MyAppName} Standalone (64-bit)"; Types: full standalone custom
Name: "DOCS"; Description: "{#MyAppName} Manual"; Types: full vst3 standalone custom

[Files]
Source: "{#MONIQUE_BIN}\monique_products\{#MyAppName}.vst3\*"; DestDir: "{autocf}\VST3\{#MyAppPublisher}\{#MyAppName}.vst3\"; Components: VST3; Flags: ignoreversion recursesubdirs
Source: "{#MONIQUE_BIN}\monique_products\{#MyAppName}.clap"; DestDir: "{autocf}\Clap\{#MyAppPublisher}\{#MyAppName}.clap"; Components: CLAP; Flags: ignoreversion
Source: "{#MONIQUE_BIN}\monique_products\{#MyAppName}.exe"; DestDir: "{app}"; Components: SA; Flags: ignoreversion
Source: "{#MONIQUE_SRC}\resources\manual\Manual-English.pdf"; DestDir: "{app}"; Components: DOCS; Flags: ignoreversion

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppName}.exe"; Flags: createonlyiffileexists
