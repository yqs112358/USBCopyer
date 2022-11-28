# USB Copyer

After inserting the USB disk, the program automatically traverses the directory and copies all the files that meet the requirements to the specified location. The copying process does not affect the normal USB disk ejection and unplugging.

## Execution

The main program is executed directly without pop-ups. If you need to close it, please use Task Manager to end the process.

When a USB disk being inserted, the program opens a new thread responsible for its work: traversing the directory and copying files to the specified location by directory level, file type, file size, etc. as limited by the configuration file.

If the USB disk is ejected during the copying process, it can be ejected normally without the occupancy prompt.

## Config file

**config.ini** will be generated when first executing the program.

```ini
[Main]
# Maximum search directory depth, set to 0 for no limit
SearchMaxDepth=5
# Delay time to wait before starting replication (seconds)
DelayStart=30
# Restrict the format of copied file extensions, with | separating the two extensions. Leave blank to indicate no restriction
FileExts=.doc|.ppt|.xls|.docx|.pptx|.xlsx|.txt|.pdf
# Limit the maximum size of file copy, available in B/KB/MB/GB, set to 0 for no limit
FileSizeLimit=1000MB
# Destination file save directory, available placeholders: <date> - date string, <time> - time string, <drivelabel> - drive volume label
SavePath=./.saved/<date>_<time>_<drivelabel>/
```

## Save & Log

In default, the save directory is `./.saved/<date>_<time>_<drivelabel>/`, and the directory `.saved` is hidden. You can access this directory by directly input its address in explorer.exe, or you can change target save path in config file.

By the way, logs will be output into directory `.saved`.
