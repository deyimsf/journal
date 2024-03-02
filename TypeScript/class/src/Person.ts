// ts中的类

class Person  {
    // 实例属性
    name:string = '张三';
    // 只读属性
    readonly name1:string = '李四';
    // 静态属性
    static name2:string
    // 静态只读属性
    static readonly name3:string;

    /**
     * 定义方法
     * 可以加static修饰
     * */
    sayHello(){
        console.log("hello")
    }
}

const person = new Person();
console.log(person)

console.log(person.sayHello())
