---
layout: post
title:  "I have a cunning plan!"
date:   2017-04-03 22:48:00 +0100
categories:
---
A JVM might seem like a vast, complex thing that is way beyond anything that anyone other than a top-notch team of programmers could implement, but that's really not that case. It's not a small project by any means, but the key to completing any seemingly insurmountable task is to do it bit by bit, like eating an elephant. In this case, Sun and Oracle have done a lot of the work for us in creating the [Java Virtual Machine Specification](https://docs.oracle.com/javase/specs/jvms/se8/html/index.html).

Like any specification, it can be used as a high-level plan for a bottom-up approach. [Chapter 2](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-2.html) in particular summarises the core components that are required for an implementation to be compatible with existing Java code. I will begin with the basic components, reading and parsing class files, implementing data types etc, and slowly build up the pieces into a (at least partially) working whole.

A sensible place to start is the parsing of class files. All compiled Java bytecode resides in class files, whether on their own or in a jar, so without being able to read them, Mimic wouldn't be able to do much.

[Chapter 4](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html) of the specification has all the information about class files. We will need to read a class file from the disk, parse out the various pieces of data, check the format of the file is correct, and finally verify that the class is valid. This is going to need a lot of testing...

The format of a class file seems simple enough. It's defined as follows (where u1, u2 and u4 are 1, 2 and 4 byte unsigned integers, respectively):
```
ClassFile {
    u4             magic;
    u2             minor_version;
    u2             major_version;
    u2             constant_pool_count;
    cp_info        constant_pool[constant_pool_count-1];
    u2             access_flags;
    u2             this_class;
    u2             super_class;
    u2             interfaces_count;
    u2             interfaces[interfaces_count];
    u2             fields_count;
    field_info     fields[fields_count];
    u2             methods_count;
    method_info    methods[methods_count];
    u2             attributes_count;
    attribute_info attributes[attributes_count];
}
```

The other structs are also listed

```
cp_info {
    u1 tag;
    u1 info[];
}

field_info {
    u2             access_flags;
    u2             name_index;
    u2             descriptor_index;
    u2             attributes_count;
    attribute_info attributes[attributes_count];
}

method_info {
    u2             access_flags;
    u2             name_index;
    u2             descriptor_index;
    u2             attributes_count;
    attribute_info attributes[attributes_count];
}

attribute_info {
    u2 attribute_name_index;
    u4 attribute_length;
    u1 info[attribute_length];
}
```

It becomes immediately apparent that we'll want a class that can take a stream of bytes and be asked for 1, 2 or 4 byte unsigned integer values at a time. It should be able to keep track of how many bytes in a stream have not been read (assuming the length is known), and it should behave predictably if the requested number of bytes cannot be read. Ideally, we'd be able to use the proposed transactional memory features of C++17 for this, but we'll have to settle for seeking ahead to ensure that the bytes are there. If not, nothing at all should be read from the stream and it should throw a `parse_failure`. Maybe later we can include the position in the stream in the exception. We'll call this class `ByteConsumer` as it consumes a stream of bytes. This kind of class is dead useful as it allows you to completely forget about all the annoying details of parsing raw bytes into the types you need, aside from handling exceptions.

For the class itself, we want... well, a class. The ancillary structs listed above can go in the header to keep everything together and the ClassFile class will need a constructor that takes an initialised `ByteConsumer`.

The last thing we need to consider before getting stuck into making the parser is the structs above. Remember above when I said that the class format seems simple? It really isn't. Each of the arrays of structs within is an array of at least 10 different types of structs, all of which have wildly differing meanings and constraints. Ugh.

Obviously the last thing we want is to deal with raw pointers and dynamic arrays, so the arrays will be vectors and the constant pool itself will be a class that only provides one templated method to retrieve a pool entry with a specific index. We do **not** need to check that the entry at the specified index matches the template argument. This is because the class is validated after parsing to ensure that all its indices are correct (among other things). That will come later.

Having figured out what to do with all the data, reading it into the structures is relatively simple and can be seen in [ClassFile.cpp](https://github.com/Jooles/mimic/commit/4843bd73e09f257f6198e5cedf0e32c71910ba26#diff-9b2654415469daec7e44ef829985451b). I've also stuck in the required checks for the magic number (which must be equal to 0xCAFEBABE) and the length of the class file (which must not be truncated or have trailing bytes after parsing). The latter makes use of the `bytesRemaining` counter in `ByteConsumer`.

As you can see in that commit, the constant pool entries are parsed into structs of the correct type for their tags. I also need to put in checks to ensure the following (specified in [§4.8](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.8)):  

* All recognized attributes must be of the proper length.
* The constant pool must satisfy the constraints documented throughout [§4.4](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.4).
* All field references and method references in the constant pool must have valid names, valid classes, and valid descriptors ([§4.3](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.3)).

None of that is particularly difficult, and will require thorough unit testing to ensure the checks function correctly. Just follow the spec :D

Next time I'll be going over the parsing of the `attributes` sections properly and some verification (there's a lot of it).

Please do comment below, and feel free to ask for clarifications.
