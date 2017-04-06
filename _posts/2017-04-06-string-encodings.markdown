---
layout: post
title:  "String encodings, or How I learned to stop worrying and love Unicode"
date:   2017-04-06 23:21:00 +0100
categories:
---
Unicode is a Good Thing™. We live in an increasingly connected world and even ten years ago it was essential that humans be able to exchange text with each other around the world. I'm not going to go into detail about the history of Unicode so if you don't know about it go and read [Joel Spolsky's excellent article on encodings](https://www.joelonsoftware.com/2003/10/08/the-absolute-minimum-every-software-developer-absolutely-positively-must-know-about-unicode-and-character-sets-no-excuses/). Go on, I'll wait.

Ok, so even _15 years ago_ when that was written it was important to write software with decent Unicode support in order to avoid easily preventable encoding problems. This problem is only getting worse as more characters are added to Unicode all the time, including the regular spate of new emoji, each of which has to have an encoding. I don't know about you, but I'd be pretty pissed if I wanted to tell someone how :hankey: they/their pets/their code/etc are and have a program tell them in no uncertain terms that they're ð��© instead, which is the [mojibake](https://en.wikipedia.org/wiki/Mojibake) you get when decoding that emoji in ISO-8859-1 (a common fixed-length, one-byte encoding for Western languages).

So what does this have to do with a JVM? Well, for some reason Sun decided that they would store strings in memory and on disk in what they call a [modified UTF-8 format](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.4.7). This is a Unicode format that can encode any character that's in the Unicode standard, but unlike the standard UTF-8 format that most things can now handle correctly, Sun's format uses a 6 byte encoding where UTF-8 would use 4 bytes.

"But Jooles!" I hear you cry, "Those 4 byte encodings in UTF-8 are only used for obscure characters in things like Japanese aren't they?". And 20 years ago you might have been right. That's not the point. Your code should be able to handle all characters. Not to mention  the fact that every new emoji added to Unicode is well above the U+0000 - U+FFFF range that fits into 3 bytes. So if you want that poop to display properly, you have to deal with encodings correctly.

Now C++ has a long and sordid history with strings. It's gotten a lot better since C++11 came out, bringing with it classes like std::u16string and std::u32string as well as some nice tools for converting between all the standard Unicode formats. Unfortunately, we can't use any of those because the modified UTF-8 used by the JVM is not standard.

As a result, I have written a class for parsing and performing string operations on modified UTF-8 strings: [JUtf8String](https://github.com/Jooles/mimic/blob/master/src/JUtf8String.h). It's far from finished, but you can see the basics. It can be created from a vector of modified UTF-8 bytes and it can convert a std::string into modified UTF-8, as well as convert modified UTF-8 back into standard UTF-8 for display.

Once I've added more functionality, like the various string operations (substring, find, etc) and some iterators I may even spin this class out into a separate library. In doing research to write it, I saw more than a few Stack Overflow posts where people have given up on using strings provided by the JNI rather than have to deal with modified UTF-8.

For now, I'm just using this class internally as I build up the constant pool, but string encodings are an interesting topic and are all too easily screwed up.