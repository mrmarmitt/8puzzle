# dist-theforge.ps1 - monta o pacote de distribuicao do jogo The-Forge.
#
# Passo SEPARADO do build (rode depois do MSBuild): copia do out/ so o que a
# maquina de destino precisa, resolve a unica dependencia externa (a fonte
# TitilliumText, que o PathStatement do build referencia na arvore do
# The-Forge irmao) e reescreve o PathStatement com mounts locais. O resultado
# em dist/<Projeto>/ e autossuficiente: descompactar e rodar.
#
# Uso:
#   powershell -File tools\dist-theforge.ps1                 # Release, 8PuzzleForgeLib
#   powershell -File tools\dist-theforge.ps1 -Zip            # + dist\8PuzzleForgeLib.zip
#   powershell -File tools\dist-theforge.ps1 -Project 8PuzzleForge
#
# Requisitos na maquina de DESTINO (ver LEIA-ME.txt gerado):
#   Windows 10/11 x64, GPU com D3D12 e o VC++ Redistributable x64.
#
# (arquivo mantido em ASCII puro: PowerShell 5.1 le .ps1 sem BOM como ANSI)

param(
    [string] $Project = '8PuzzleForgeLib',
    [string] $Configuration = 'Release',
    [switch] $Zip
)

$ErrorActionPreference = 'Stop'

$repoRoot = Split-Path -Parent $PSScriptRoot
$outDir = Join-Path $repoRoot "out\theforge\x64\$Configuration\$Project"
$theForgeFonts = Join-Path $repoRoot "..\The-Forge\Art\Fonts\TitilliumText"
$distDir = Join-Path $repoRoot "dist\$Project"

# --- validacoes -------------------------------------------------------------
if (-not (Test-Path (Join-Path $outDir "$Project.exe"))) {
    throw "Nao achei $Project.exe em $outDir - builde antes: MSBuild src\platform\theforge\PC_VS2019\$Project.vcxproj /p:Configuration=$Configuration /p:Platform=x64"
}
if (-not (Test-Path (Join-Path $theForgeFonts 'TitilliumText-Bold.otf'))) {
    throw "Nao achei a fonte em $theForgeFonts (o checkout do The-Forge precisa estar ao lado do repo)"
}

# --- pasta limpa ------------------------------------------------------------
if (Test-Path $distDir) {
    Remove-Item -Recurse -Force $distDir
}
New-Item -ItemType Directory -Force $distDir | Out-Null

# --- binarios e configs (SEM artefatos de dev: pdb/lib/exp/log, camada de
#     debug do D3D12, e sem o records.tsv do desenvolvedor) ------------------
$files = @(
    "$Project.exe",
    'gpu.cfg',
    'gpu.data',
    'D3D12Core.dll',        # Agility SDK (ativado pelos exports do exe)
    'dxcompiler.dll',
    'amd_ags_x64.dll',
    'WinPixEventRunTime.dll'
)
foreach ($file in $files) {
    Copy-Item (Join-Path $outDir $file) $distDir
}

# shaders compilados no build (rootsigs por-app + fontstash do projeto OS)
Copy-Item -Recurse (Join-Path $outDir 'CompiledShaders') (Join-Path $distDir 'CompiledShaders')
Copy-Item -Recurse (Join-Path $outDir 'Shaders') (Join-Path $distDir 'Shaders')

# --- a fonte, agora DENTRO do pacote (com a licenca OFL, exigida por ela) ----
New-Item -ItemType Directory -Force (Join-Path $distDir 'Fonts\TitilliumText') | Out-Null
Copy-Item (Join-Path $theForgeFonts '*') (Join-Path $distDir 'Fonts\TitilliumText\')

# --- PathStatement com todos os mounts locais (RD_FONTS era o unico que
#     apontava para fora e e usado; os de Art/ nao sao carregados pelo jogo) --
$pathStatement = @'
RD_SHADER_BINARIES = CompiledShaders/
RD_PIPELINE_CACHE = PipelineCaches/
RD_TEXTURES = Art/Textures/
RD_COMPILED_MATERIALS = CompiledMaterials/
RD_MESHES = Art/Meshes/
RD_FONTS = Fonts/
RD_ANIMATIONS = Art/Animations/
RD_AUDIO = Art/Audio/
RD_SCRIPTS = Scripts/

RD_SCREENSHOTS = Screenshots/
RD_DEBUG = Debug/
'@
# sem BOM: o parser C do The-Forge le a primeira chave do inicio do arquivo
[System.IO.File]::WriteAllText((Join-Path $distDir 'PathStatement.txt'), $pathStatement)

# --- pastas que o runtime usa para ESCREVER (mesmas do post-build); o .keep
#     as mantem vivas dentro do zip (Compress-Archive descarta pasta vazia) ---
foreach ($dir in @('PipelineCaches', 'Debug')) {
    New-Item -ItemType Directory -Force (Join-Path $distDir $dir) | Out-Null
    New-Item -ItemType File (Join-Path $distDir ($dir + '\.keep')) | Out-Null
}

# --- instrucoes para quem recebe ---------------------------------------------
$readme = @"
8Puzzle (The-Forge) - $Project
==============================

Como jogar: execute $Project.exe (nao precisa instalar).

Requisitos:
- Windows 10/11 x64 com GPU compativel com Direct3D 12
- Visual C++ Redistributable x64 (se faltar, o Windows acusa
  "VCRUNTIME140.dll nao encontrado"): https://aka.ms/vs/17/release/vc_redist.x64.exe

Controles: setas movem a peca vizinha do vazio (1-8 move pelo numero),
ENTER confirma, ESC volta/sai, T/M ordenam os recordes.

Os recordes ficam em records.tsv, criado ao lado do exe na primeira vitoria.

Fonte TitilliumText (c) Accademia di Belle Arti di Urbino - SIL Open Font
License (Fonts/TitilliumText/OFL.txt).
"@
[System.IO.File]::WriteAllText((Join-Path $distDir 'LEIA-ME.txt'), $readme)

# --- zip opcional -------------------------------------------------------------
if ($Zip) {
    $zipPath = Join-Path $repoRoot "dist\$Project.zip"
    if (Test-Path $zipPath) {
        Remove-Item -Force $zipPath
    }
    Compress-Archive -Path $distDir -DestinationPath $zipPath
    Write-Host "zip:  $zipPath"
}

$sizeMB = (Get-ChildItem -Recurse $distDir | Measure-Object Length -Sum).Sum / 1MB
Write-Host ("dist: {0}  ({1:N1} MB)" -f $distDir, $sizeMB)
