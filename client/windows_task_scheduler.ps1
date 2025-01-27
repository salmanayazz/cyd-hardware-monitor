# current directory
$currentDir = (Get-Location).Path
# current dir + script name
$scriptPath = Join-Path -Path $currentDir -ChildPath "main.py"

# task parameters
Write-Output "Creating action..."
$action = New-ScheduledTaskAction -Execute "cmd.exe" -Argument "/c start /min python.exe $scriptPath" -WorkingDirectory $currentDir
Write-Output "Action created: $($action.Execute) $($action.Arguments) - Working Directory: $currentDir"

$trigger = New-ScheduledTaskTrigger -AtStartup
$principal = New-ScheduledTaskPrincipal -UserId "SYSTEM" -LogonType ServiceAccount
$settings = New-ScheduledTaskSettingsSet -StartWhenAvailable

# register task
Register-ScheduledTask -TaskName "hardware-monitor-client" -Action $action -Trigger $trigger -Principal $principal -Settings $settings

Write-Output "Scheduled task created successfully!"
