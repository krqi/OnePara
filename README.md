# OnePara
支持任意类型及结构的C++通用数据结构，从繁杂的数据结构定义中解放出来。可以接受处理任意形式的数据结构及数据类型，简单如单个任意类型数据int/string/char/float等，及数组、各种树及以上各种的混合嵌套，当然也很容易和xml、json等等兼容相互导入导出，添加了bson的支持，可以通过bson兼容json及xml（也可以很容易添加其他）。
C ++ generic data structures that support any type and structure are freed from the complex data structure definition. Can accept any form of data structures and data types, as simple as a single arbitrary type of data int / string / char / float, and arrays, a variety of trees and a variety of mixed nested, of course, is very easy and xml, json Etc. Compatible with each other to import and export, add bson support, compatible with json and xml through bson (also can easily add other)

example:
```
    OnePara in, out;
    in["number"] = 123;
    in["table"] = "user";
    in["data.email"] = "user@13.com";
    in["data.CreateTime"] = "1234567";
    in["data"]["who"] = "sunny";
    
    out = in;
    out.display("outshow");
    out["change"] = in["data"];
    out.display("outshow2");
    
    in["out"] = out;
    in.display("in");
    
    in.remove("data.who");
    in.display("in");
    in.clear();
    in.display("in");
    
    out.clear();

```
