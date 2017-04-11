---
layout: post
title:  "Piece by piece"
date:   2017-04-11 23:36:00 +0100
categories:
---
I was planning on trying to get the Class file structure validation done before my next post, but I realised it would be worth talking about how I've been working. That and I had to take a bit of a break from reading the Java specification. Yeesh.

If anyone's been following along on GitHub, you might have noticed that I seem to have been going off on a bit of a tangent. Lets look at the commits that have been pushed since the last post:

```
746ef6f Add split, contains and find methods to JUtf8String
c00109a Fix postfix increment implementation in JUtf8String iterator and use prefix
2a5e00a Replace charAt and nextIndex with forward iterator in JUtf8String
7dd7b09 More tidying and add preparatory structures ready for attribute parsing
0e8ae8f Remove unnecessary variables, tidy up JUtf8String and add number classes
f2a5865 Make an INVALID static member for invalid CP entries and remove std::couts
```

The general theme is that having reached a decent stopping point, I've been going over what I've just written in order to make the code neater, less confusing, and generally tidier. I've added usability improvements, like using a standard iterator in the string class and used a specific type that can only ever be invalid for entries in the constant pool which are, well... invalid.

No code is ever perfect, but it's really important to regularly try and make it, if not perfect then as well written as possible. I'm reminded of advice I was given about writing essays in exams, as it applies equally well to programming. Always leave time for editing. If your code is convoluted, confusing, hard to read, not thoroughly tested or anything like that then it *will* come back to bite you in the ass later. I've learned that the hard way.

There's also the addition of a few functions to the JUtf8String class. Aside from the constant pool, most of the class file is built on string identifiers, encoded in Java's modified UTF-8, so to be able to properly work on the rest of the data, we need to be able to do more with these strings.

Finally, I've mentioned testing above but haven't really been talking about it much. You can see that all code I write comes with quite a few tests, and can probably surmise that I'm a firm believer in Test Driven Development. If you're not already a convert then I cannot recommend trying it enough.

Just as there are different approaches to design (top down, bottom up, etc), there are a number of different approaches to TDD, all of which have pros and cons. Some say to start with unit tests and to never write a line of production code that isn't to make a test pass. Others will say to write behavioural tests of the public interface(s), be they console input/output, APIs, signals, etc., and to implement the required functionality from there. Whatever way works for you is fine, unless of course a workplace enforces a specifc discipline of course. This goes for the tools you choose as well.

All that said, if you use emacs you're dead to me ;)

I'm not sure when the next post will be, as there's a bunch of auxiliary classes required before I can move onto the next step of validating the structure of a parsed class file, but I'm making steady, if slow, progress. Loading class files was perhaps a long and arduous place to start, but once it's done, there's just the fun stuff to do, so bear with me. Each little piece written takes the project one step closer to being functional.