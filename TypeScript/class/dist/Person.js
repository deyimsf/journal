"use strict";
// ts中的类
class Person {
    constructor() {
        // 实例属性
        this.name = '张三';
        // 只读属性
        this.name1 = '李四';
    }
    /**
     * 定义方法
     * 可以加static修饰
     * */
    sayHello() {
        console.log("hello");
    }
}
const person = new Person();
console.log(person);
console.log(person.sayHello());
