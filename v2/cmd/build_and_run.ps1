Write-Host -ForegroundColor DarkBlue " * aiva: starting project file generation..."
premake5 gmake2 --cc=clang

if ($LASTEXITCODE -eq 0)
{
    Write-Host -ForegroundColor DarkBlue " * aiva: project file generation completed successfully."
}
else
{
    Write-Host -ForegroundColor DarkRed " * aiva: project file generation failed with exit code $LASTEXITCODE"
    exit $LASTEXITCODE
}

Write-Host -ForegroundColor DarkBlue " * aiva: starting build process..."
mingw32-make

if ($LASTEXITCODE -eq 0)
{
    Write-Host -ForegroundColor DarkBlue " * aiva: build process completed successfully."
}
else
{
    Write-Host -ForegroundColor DarkRed " * aiva: build process failed with exit code $LASTEXITCODE"
    exit $LASTEXITCODE
}

Write-Host -ForegroundColor DarkBlue " * aiva: starting execution of the application..."
.\bin\aiva.exe

if ($LASTEXITCODE -eq 0)
{
    Write-Host -ForegroundColor DarkBlue " * aiva: application execution completed successfully."
}
else
{
    Write-Host -ForegroundColor DarkRed " * aiva: application execution failed with exit code $LASTEXITCODE"
    exit $LASTEXITCODE
}
