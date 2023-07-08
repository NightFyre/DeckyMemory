# DeckyMemory
Customized memory library for use with the SteamDeck.
<p align="center">
<img src="https://github.com/NightFyre/DeckyMemory/assets/80198020/83fa8e55-eb8a-4661-a37f-c57faba0843f.png">
</p>

# GOALS
Establish a simplistic means of accessing and manipulting process memory on the Steam Deck from an external process.
- Obtain Process Information
- Read & Write Process Memory
- Change Protections & Allocate Memory
- Inject Shared Library

# COLLABORATING GUIDELINES
Please review the following section guidelines if you would like to contribute to the project.
- Pull from the latest branch before submitting a pull request
- Please do your best to document any and all changes along with reasoning
- Follow Class, Structure and variable guidelines

## NEW STRUCTURE TEMPLATE
```cpp
    typedef struct S<StructName>
    {
        // Member variables
    } <StructSecondary>, <StructPointer>; // Size: 
```

## NEW CLASS TEMPLATE
```cpp
    /*
    * Class DeckyMemory.C<ClassName>
    * Size -> 0x0000 (FullSize[0x0000] - InheritedSize[0x0000])
    */
    class C<ClassName>
    {
    private:	//	"_<VarName>" PRIVATE MEMBER VARIABLES
    public:     //  "<VarName>" PUBLIC MEMBER VARIABLES      
    public:	    //	"_<FncName>" CUSTOM FUNCTIONS for Private Memmbers
    public:	    //	"<FncName>" PUBLIC MEMBER FUNCTIONS
    };  //  Size: 0x0000
```
