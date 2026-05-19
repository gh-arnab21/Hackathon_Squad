param (
    [string]$TargetFile = "input_*.txt",
    [int]$TimeLimit = 295
)

Write-Host "Compiling mwis.cpp..."
g++ -O3 -std=c++17 mwis.cpp -o mwis.exe
if ($LASTEXITCODE -ne 0) {
    Write-Host "Compilation failed!"
    exit 1
}

$files = Get-ChildItem -Filter $TargetFile | Sort-Object Name
foreach ($file in $files) {
    $outFile = $file.Name -replace '^input_', 'output_'
    if (-not (Test-Path $outFile)) {
        continue
    }

    # Read target score
    $oldLines = Get-Content $outFile
    $oldScore = 0
    if ($oldLines.Count -gt 0 -and $oldLines[0] -match '^\d+$') {
        $oldScore = [double]$oldLines[0]
    }
    else {
        continue
    }

    Write-Host "====================================================="
    Write-Host "Running $($file.Name) for $TimeLimit seconds..."
    
    Get-Content $file.FullName | .\mwis.exe $TimeLimit > temp_out.txt
    
    $newLines = Get-Content temp_out.txt
    $newScore = 0
    if ($newLines.Count -gt 0 -and $newLines[0] -match '^\d+$') {
        $newScore = [double]$newLines[0]
    }

    if ($newScore -eq $oldScore) {
        Write-Host "ACCEPTED" -ForegroundColor Green
        Write-Host "my score : $newScore | Target score: $oldScore | accuracy:100%" -ForegroundColor Green
    }
    elseif ($newScore -gt $oldScore) {
        $acc = ($newScore / $oldScore) * 100
        Write-Host "ABOVE TARGET" -ForegroundColor Cyan
        Write-Host "my score : $newScore | Target score: $oldScore | accuracy: $([math]::Round($acc, 4))%" -ForegroundColor Cyan
    }
    else {
        $diff = (($newScore - $oldScore) / $oldScore) * 100
        Write-Host "BELOW TARGET" -ForegroundColor Red
        Write-Host "my score : $newScore | Target score: $oldScore | accuracy: $([math]::Round($diff, 4))%" -ForegroundColor Red
    }
}
Remove-Item temp_out.txt -ErrorAction SilentlyContinue
Write-Host "All tests completed."
