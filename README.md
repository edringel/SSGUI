# SSGUI
Small Screen GUI Manager
SSGUI is an Arduino library that simplifies maintaining a graphical user interface on a small (i.e., 7 inch and less) screen. Right now, at least, it is very much a work in progress. However, looks kinda promising...

Rough cut on the architecture and operational principles:

The main class is SSGUIScreen. It contains the graphical and interactive interface, and methods for controlling/manipulating graphics and interacting with the user.

The SSGUIScreen object (there is only one) maintains a vector list of ScreenObj's. This is a base class that contains information about a graphical element that is common to any and all graphical elements. It also contains the information needed to respond to a touch within the object's boundaries.

Actual graphical elements (circle, rectangle, line of text) are expressed as child classes of ScreenObj. They contain the generic object information contained in a ScreenObject as well as the information specific to that graphic element. For example, a rectangle needs an origin coordinate, a width, and a height. The origin will be the top-right point. A circle, however, needs a center coordinate and a radius. And so on.

Generic ScreenObj information includes colors, etc. Each object has a unique identifier number, a code that identifies its subclass, and whether or not it should respond to a touch within its boundaries (act like a control.) Additionally, each object is tagged as belonging to a screen (on a larger display one might call it a window). For example, a clock might have a main display screen, but also have additional screens for various settings. By tagging objects as (virtually) belonging to a screen, it becomes very easy to change to a new set of graphical elemnts. Additionally, from the standpoint of responding to a touch, the screen information can be used to filter responses (only respond to a touch within an object's boundaries if its screen is active.) Having a ScreenObj belong to a specific screen allows a touch on the same piece of real estate to have a different function without a lot of fuss on the part of the user of the library.

Information about the objects is contained in a file which is kept at the root directory of an SD card. The file has a specific structure, with some information global to the SSGUIScreen object. Each object then has its own line of text that contains ScreenObject parameters and then information specific to the circle/rectangle/text/etc subclass.

The SSGUIScreen class requires three other libraries to work. First, there must be an LCD object. I have been developing using an ILI_9341 unit, using the Adafruit library. Second, there must be a touchscreen object. The XPT_2046 controller is used on my device. Finally, the Arduino must be able to access an SD card. I have been using a Teensy 3.6 which handles this nicely.  After futzing with this project for a bit, I found it easiest to instantiate the screen and touch objects within the library, similar to Serial and SD are handled. Thus, use of the library automatically creates a graphics object, a touch object, and the small screen GUI object (tft, ts, and SSG respectively).  I would caution the reader/potential user that graphics and user interaction are resource hungry. This ain't gonna fly on an Arduino mini, and I think that units such as the ESP series, the Teensy 3.x series, and the Due and clones are more in the ballpark

OK, so how does this all actually work?

By including the library in your sketch, and including the SD library, you will have the basic objects needed. In setup(), the program should call SSG.begin(whichScreen). The code looks for and opens a file names "rsrc.txt" on the SD card and parses it into global and object specific information. The various screen objects are instantiated and the information is stored. The whichScreen active screen is then drawn.

The main event loop, at least as far as the SSG object is concerned, is very simple. Include SSG.manageTouch(). The function returns zero if no object has been touched, or the unique ID number of a touched control-enabled object. As I said above, the manageTouch() function not only looks for a touched control object, but also only returns its number if its screen is active. Response to the touch is then handled by a swtich statement, with each case corresponding to the unique ID of the touched object.

Manipulation of screen contents is also quite easy. An object is drawn simply by calling SSG.drawObject(UniqueID). An entire screen can be drawn by calling SSG.drawScreen(ScreenID). Alteration of color, text content, and an object's control status with a series of maintenance functions. See more on this below.

Portability:

LCD screens and Touch Screens and processors obviously all have different needs. However, the fundamental information needed by most graphics libraries to draw a rectangle or circle or present text is pretty much the same. Device specific calls are isolated to only a few methods and classes. When all is said and done most of the device specific heavy lifting is done in the drawObj() method, and theoretically drawObj and manageTouch might be the only methods that needed LCD and Touchscreen specific calls. Information in the SSGUI1.h header file would be needed as well to use the correct #includes and to set the pins. SSG instantiation would need no changes, but tft and ts would need to be updated.

So, to use this library as is:
  1. Copy rsrc.txt to the root directory of an SD card or microSD card
  2. Make sure that you have the Adafruit GFX library and ILI_9341 library, as well as the SD library.
  3. Review the pin assignments in SSGUI1.h and make sure they fit with your processor, particularly with respect to SPI
  4. Wire up your breadboard, and fire away. Note that the ILI9341 is a cranky little beast. DO NOT just set the Reset line to high. Use a reset output from the processor. The creature really wants to be reset every time the program is rerun.
  
Where this gets really cool:

    Want to add a new graphic element? Easy. Just alter your rsrc.txt file. NO NEW CODE!!! Obviously, if you add an element with control potential you will need to update your swtich statement, but that's it.
  
    Don't like the position of one of your graphic elements? Easy. Change the parameters in the rsrc.txt file.
  
   Want to make a new kind of screen object? Medium easy. Subclass ScreenObj, write a constructor for the new subclass, and update DrawObj to include instructions for the new object. This is not as bad as it sounds. Because an actual screen object is a subclass of ScreenObj, common information is already parsed. In all likelihood, you will only need to extract a few parameters. I've written a helper function that makes it pretty easy. I'll add more documentation later, but I think if you look through the SSG.begin() method you'll see that the method reads a line of text from the file and removes the text that has been read/used as each ScreenObj parameter is extracted. By the time your specialized constructor needs to be called, the text string contains only what you need to complete construction (see the various constructors). Each specialized constructor is also responsible for "backfilling" the ScreenObj information about the boundary rectangle used to determine if the object has been touched. Updating the DrawObj routine will generally consist of providing a wrapper around a preexisitng graphics library call, unless you are doing something really snazzy.
  
Structure of rsrc.txt

   First line: Screen size (Y, X) mostly for future use
  
   Second line: number of screen objects
  
   Subsequent lines: data for each screen object. Comma separated, numbers are tolerant of spaces. Text should be EXACTLY what you want to see on the screen, starting immediately after the preceding comma. ,"Hello" will show up as "Hello".  , Hello will show up as Hello with a preceding space. ,Hello will show up as Hello.  Here are the structures:
  
   First position: Unique ID
   Second position: "Owner" screen
   Third position: object type. so far, I've defined three: 1 = rectangle, 2 = circle, 3 = text
   Fourth position: Is a control.  0 = no, 1 = yes.
   Fifth, sixth, and seventh position: Forecolor, back color, and fill color. Fore color would be the color generally used the draw the outline of the object. Back color is used particularly for text, but is mandatory for all object types. Might be used in the future. Fill color is for filling objects, such as a rectangle or circle. If back color != fill color, the object will be filled with the fill color.
  
  Positions 8 and beyond are specific to the object.
  
     Rectangle: top left X, top left Y, width, height
     Circle: center X, center Y, radius
     Text: top left X, top left Y, text size (as in tft.setTextSize()), text string
    
    
    More to come, I hope.
