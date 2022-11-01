# Inheritance

- BufferRef
    - Buffer

- ObjectRef
    - Object

- Type
    - Fundamental
    - Enum
    - Pointer
    - Class
        - TemplateInstance

- Member
    - Field
    - Property

- Function
    - Method
    - Constructor

- AttributeBase

# Functionality

- BufferRef
    todo 
        - implement copy
        - add unit tests

    - check is valid
    - get size
    - get data ptr
    - steal data ptr
    - copy data (returns Buffer)

- Buffer : BufferRef
    todo 
        - implement change buffer size
        - implement release buffer
        - implement call constructor
        - add unit tests

    - change buffer size
    - release buffer
    - call constructor (returns Object)

- ObjectRef
    todo
        - implement copy assign
        - implement move assign
        - add unit tests

    - check is valid
    - get type
    - get size
    - get value ptr
    - steal value ptr
    - copy assign
    - move assign

- Object : ObjectRef
    todo 
        - implement change pointed object
        - implement release object
        - implement call destructor
        - add unit tests

    - change pointed object
    - release object
    - call destructor (returns buffer)

- Function
    todo - implement the whole type

    - get name
    - get return type
    - get params
    - call

- FunctionParam
    todo - implement the whole type

    - get name
    - get type

- Type
    todo 
        - use res as return value for all applicable methods
        - implement get cv qualifiers
        - implement get ref

    - get name
    - get size
    - get attributes
    - get cv qualifiers
    - is ref
    - new
        - default
        - copy
        - move
    - delete
    - construct
        - default
        - copy
        - move
    - destruct
    - assign
        - copy
        - move

- Fundamental : Type

- Enum : Type
    todo - implement get value and set value 

    - get values
    - get value
    - set value

- EnumValue
        - get name
        - get value

- Pointer : Type
    todo - implement the whole type

- Class : Type
    todo 
        - implement getter for base classes
        - implement getter for members and functions by name
        - implement constructor function type

    - get base classes
    - get non default constructors
    - get members
    - get methods

- Member
    todo - implement the whole type

    - get name
    - get type
    - get declaring class
    - copy value
    - copy assign

- Field : Member
    todo - implement the whole type

    - get offset
    - get value
    - steal value
    - set value
    - move assign

- Property : Member
    todo - implement the whole type

- Method : Function
    todo - implement the whole type

    - get declaring class
    - call on
    
- TemplateInstance : Class
    todo - implement the whole type

    - get template params
    - get template class name
    - get template name

- TemplateParam
    todo - implement the whole type

    - get name
    - get type

Script