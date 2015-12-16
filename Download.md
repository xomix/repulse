# Downloading repulse #

## Download the latest release ##
> There is no release.

## Compiling ##

After downloading you can compile a binary like this:
```
tar -xvf repulse-0.0.1.tar.bz2
cd repulse-0.0.1
./build.sh
```

You can go on and execute repulse like this:
```
./repulse -c patch.xml
```

## Dependencies ##

  * ALSA http://www.alsa-project.org
  * Jack http://jackaudio.org
  * SndFile http://www.mega-nerd.com/libsndfile
  * Secret Rabbit Code (libsamplerate) http://www.mega-nerd.com/SRC

## Downloading the SVN version ##

Use this command to anonymously check out the latest project source code:
```
svn checkout http://repulse.googlecode.com/svn/trunk/ repulse
```