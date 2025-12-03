## Moved to Codeberg

## Thm-match
A system for identifying duplicate images.

### Synopsis

__tmatch__ [-dimvVz] _searchlist database_

__tfind__ [-finosuvVx] _dataset_

__tdiff__ _imageA_ _imageB_

__tupdate__ [-dfiUvVx] _database_ [-o update-file]

__tdup__ [-dmuvVz] [-fFlLnNoO] _database_

__tcheck__ [V] _search-file_ [-d _database-file_]

### Description
__thm-match__

__tmatch__

__tfind__

__tdiff__

__tupdate__

__tdup__

__tcheck__


### Development
After writing sha-match there were still a few things that annoyed me about duplicate files - pictures, video and audio.  Relying on SHAsums for anything lossy is useless.  Just a change in metadata or resaving a jpg will kill any chance of a match with checksums/hashes.

I could see a germ of an approach, but needed to find a way.  That way came in bits and pieces from things I saw and learned over a couple of years.

### Purpose
There are duplicate image finders around.  But they are usually annoying in some way - particularly in that they don't keep a record of deleted images.  This isn't a fatal flaw, but it's something I'd find useful in the long term.

A common strategy for duplicate image finders is to summarize images using a small 1 bit per channel block (say 8x8) as a shortcut to finding similar images.  It is usually a heavily processed form of the original image.  That way you can eliminate a vast majority of comparisons and concentrate on images that are at least vaguely similar.

It creates thumbprints - 9 character summaries - of images and stores them in a database to allow searching for duplicates.


### In depth
#### tmatch

#### tfind
__tfind__ works a little like `find (1)`, building a list of files in the current directory (or all sub directories too) and uses ImageMagick to generate a 64x64 pixel, mid-grey padded, 6 bits per channel thumbnail.  The thumbnail is then analysed to provide a greyprint and hueprint and combined with the original image size to provide a thumbprint of 9 base 64 characters.  It gives the option to save the thumbnail in a subdirectory.  All the thumbprints are then saved in a textfile database.

#### supdate
__tupdate__

#### tdiff
__tdiff__ compares two thumbnails.

#### sdup
__tdup__ searches for duplicates in a database.

#### scheck
__scheck__ 

### Files

### Restrictions
The search in __smatch__ requires that the _searchlist_ is sorted by _SHA256_.  If it isn’t already, __smatch__ will sort it.  The sort is slow.  As you’re likely to generate a ___dataset___ once but use it with __smatch__ multiple times, __sfind__ sorts its output by _SHA256_ checksums by default.  That sort is also slow.  The search itself, however, is fast.

### Portability
I used only standard _C_ functions for all the programs.  Both __tfind__ and others use popen() to get thumbnails from ImageMagick.
All the programs build in _GNU/Linux_.

__tfind__ and others use _dirent.h_ to mimic the _GNU/Linux_ `find` command, building a list of files/directories in the current directory to generate thumbnails/thumbprints for.

### Bugs
I'm not a natural coder.  My debugging strategy mainly consists of putting in heaps of _printf_ statements everywhere to try to figure out what the hell is going wrong.  These debugging aids were kept on as verbose mode.  Crashes in these programs now seem to be related to trying to close a file that isn't open or due to my misunderstanding the correct use of _malloc_ or _free_.  __tfind__ sends filenames to ImageMagick enclosed in double quotes to account for spaces and other non-standard characters, but that sometimes fails - with \` (backtick) for example.

### Copyleft
If you want to steal this, I'll meet you in the carpark.

### Author
Bob Hayward (Zax) zx81@zx81.nz
