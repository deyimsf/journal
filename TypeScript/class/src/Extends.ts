import {ParsedQs} from "qs";
import {RequestHandler, RouteParameters} from "express-serve-static-core";

class Animal {
    name:string;
    age:number;

    constructor(name:string, age:number) {
        this.name = name;
        this.age = age;
    }

    sayHello(){
        console.log("hello")
    }
}

class Snake extends Animal{
    run(){
        console.log(`${this.name}在跑路`)
        console.log(this.name,'在旺旺跑')
    }
}

class Cat extends Animal{
    sayHello() {
        super.sayHello();
        console.log('我是Cat')
    }
}

class CatSuper extends Animal{
    gender: string;

    constructor(name:string, age:number, gender:string) {
        // 如果在子类写了构造函数，则必须在构造函数中用super调用父类构造函数
        super(name,age);
        this.gender = gender;
    }
}

const snake = new Snake('旺财',18);
const cat = new Cat('小黑',19)
const catSuper = new CatSuper('a',12,'ccc');

console.log(snake.run())
console.log(cat)



/**
 * 抽象类
 */
abstract class Animal1 {
    name:string;

    constructor(name:string) {
        this.name = name;
    }

    /**
     * 抽象方法
     */
    abstract sayHello():void;
}

class Dog1 extends Animal1{
     sayHello() {
         console.log('Dog1 sayHello')
     }
}


/**
 * 接口可以当成类型声明用
 * 并且可以重复声明(尽量别这样用，不直观)
 */
interface myInter {
    name:string;
}

interface myInter {
    age:number
}
const obj:myInter = {
    name: '张三',
    age: 18
}
console.log(obj)



/**
 *  接口
 */
interface PersonInterface {
    name:string;
    sayHello():void;
}

class MyClass implements PersonInterface{
    name:string;

    constructor(name:string) {
        this.name = name;
    }

    sayHello():void{
        console.log(this.name)
    }
}


/**
 * 属性封装时可用的修饰符
 *    public
 *    private
 *    protected
 */
class Person3{
    private name:string;
    private _name1:string = '';

    constructor(name:string) {
        this.name = name;
    }

    // 常规getter setter方法
    getName():string{
        return this.name;
    }
    setName(name:string):void{
        this.name = name;
    }

    //TS中的getter setter方法，前提似乎属性以 _ 开头
    get name1(){
        return this._name1;
    }
    set name1(name:string){
        this._name1 = name;
    }


}
const person3 = new Person3('李四')
console.log(person3.getName())


/**
 * 可以直接将属性定义在构造函数中
 */
class C{
    constructor(public name:string) {
    }
}
const c = new C('张三')
console.log(c.name)


/**
 * 泛型
 *   fn<T> 表示fn函数定义了一个泛型T
 */
function fn<T>(a:T):T {
    return a;
}
let res = fn(10) // 可直接用，让ts自己推断
let res1 = fn<string>('hello')  // 指定具体类型


/**
 * 泛型约束
 *    表示T必须是BigInteger的子类
 */
function fn2<T extends BigInteger>(a:T):number {
    return a.length;
}

//
// interface Abc<T>{
//     <Route extends string>(path: Route): T;
// }



