class Animal {
    constructor(name, age) {
        this.name = name;
        this.age = age;
    }
    sayHello() {
        console.log("hello");
    }
}
class Snake extends Animal {
    run() {
        console.log(`${this.name}在跑路`);
        console.log(this.name, '在旺旺跑');
    }
}
class Cat extends Animal {
    sayHello() {
        super.sayHello();
        console.log('我是Cat');
    }
}
class CatSuper extends Animal {
    constructor(name, age, gender) {
        // 如果在子类写了构造函数，则必须在构造函数中用super调用父类构造函数
        super(name, age);
        this.gender = gender;
    }
}
const snake = new Snake('旺财', 18);
const cat = new Cat('小黑', 19);
const catSuper = new CatSuper('a', 12, 'ccc');
console.log(snake.run());
console.log(cat);
/**
 * 抽象类
 */
class Animal1 {
    constructor(name) {
        this.name = name;
    }
}
class Dog1 extends Animal1 {
    sayHello() {
        console.log('Dog1 sayHello');
    }
}
const obj = {
    name: '张三',
    age: 18
};
console.log(obj);
class MyClass {
    constructor(name) {
        this.name = name;
    }
    sayHello() {
        console.log(this.name);
    }
}
/**
 * 属性封装时可用的修饰符
 *    public
 *    private
 *    protected
 */
class Person3 {
    constructor(name) {
        this._name1 = '';
        this.name = name;
    }
    // 常规getter setter方法
    getName() {
        return this.name;
    }
    setName(name) {
        this.name = name;
    }
    //TS中的getter setter方法，前提似乎属性以 _ 开头
    get name1() {
        return this._name1;
    }
    set name1(name) {
        this._name1 = name;
    }
}
const person3 = new Person3('李四');
console.log(person3.getName());
/**
 * 可以直接将属性定义在构造函数中
 */
class C {
    constructor(name) {
        this.name = name;
    }
}
const c = new C('张三');
console.log(c.name);
/**
 * 泛型
 *   fn<T> 表示fn函数定义了一个泛型T
 */
function fn(a) {
    return a;
}
let res = fn(10); // 可直接用，让ts自己推断
let res1 = fn('hello'); // 指定具体类型
/**
 * 泛型约束
 *    表示T必须是BigInteger的子类
 */
function fn2(a) {
    return a.length;
}
export {};
//
// interface Abc<T>{
//     <Route extends string>(path: Route): T;
// }
