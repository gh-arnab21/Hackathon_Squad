param (
    [string]$TargetFile = "input_*.txt",
    [int]$TimeLimit = 295,
    [string]$OutDir = "final_submission_outputs"
)

if (-not (Test-Path $OutDir)) {
    New-Item -ItemType Directory -Path $OutDir | Out-Null
}

Write-Host "Compiling mwis.cpp..."
g++ -O3 -std=c++17 mwis.cpp -o mwis.exe
if ($LASTEXITCODE -ne 0) {
    Write-Host "Compilation failed!"
    exit 1
}

$files = Get-ChildItem -Filter $TargetFile | Sort-Object Name
foreach ($file in $files) {
    $outName = $file.Name -replace '^input_', 'output_'
    $outPath = Join-Path $OutDir $outName
    
    Write-Host "====================================================="
    Write-Host "Running $($file.Name) for $TimeLimit seconds..."
    
    # Run the executable and directly stream the formatted output to the new folder
    Get-Content $file.FullName | .\mwis.exe $TimeLimit > $outPath
    
    # Read the generated score to print to the console
    $score = 0
    if (Test-Path $outPath) {
        $lines = Get-Content $outPath
        if ($lines.Count -gt 0 -and $lines[0] -match '^\d+$') {
            $score = [long]$lines[0]
        }
    }
    
    Write-Host "Done! Saved to $outPath (Score: $score)" -ForegroundColor Green
}

Write-Host "`nAll tests completed successfully."
Write-Host "You can find all formatted output files in the '$OutDir' directory."
