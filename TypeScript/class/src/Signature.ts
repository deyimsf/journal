

/***索引签名**/
interface MyArray {
    [index: number]: string
}
let array1: MyArray = ['a','b']
let array2: MyArray = {0:'a',1:'b'}
console.log(array1[0])
console.log(array2[0])


/***
 * 一旦定义了任意属性(索引签名)，那么确定属性和可选属性的类型都必须是它的类型的子集
 * 一个接口中只能定义一个任意属性。如果接口中有多个类型的属性，则可以在任意属性中使用联合类型
 *   联合类型就是在 [] 后加 |
 *
 * 这里接口的概念不同于其它语言，不必显式实现，只表示一种类型约束
 * **/
interface MyInter {  /** 等同于 type MyInter = {}   **/
    [key: string]: number|string|((input:number)=>number);
    age: number;
    // fd:4;
    name:string;
    check(s:number):number;
}
const abcObj: MyInter = {
    age:18,
    // fd:4,
    name: '张三',
    check(n:number):number {
        return 1;
    }
}
console.log(abcObj.fd, abcObj.check(3))


/**
 * 索引签名约束的interface无法被implements,因为此时它指表示类型约束，不表示其它语言中的接口？ TODO
 */
interface MyInter2 {
    [key: string]: number;
}
class MyClass implements MyInter2{
    age:number=18;
    check():void{
    }
}

/**
 * 接口也能用来表示函数类型，通过调用签名（call signature）来描述：
 */
interface SearchFun {
    // (ina:string, inb:number):boolean;
    (inc:string): number; // ?????
}

let mySearch: SearchFun = (s:string)=>{
                                return 2
                    }




//
//
// class MyClass{
//     [s:string]: number | ((s:string)=> boolean) | city:string;
//
//     age:number = 18;
//     city:string = '北京';
//
//     check(s:string){
//         return true;
//     }
//
// }

//
// interface  D{
//     [key:string]: number;
//     name:string
// }
//
// let objD: D = {
//     age:number = 19,
//     name:'张三'
// }
//
// let objx: {name:string,[key:string]: number};
//     objx = {name: '八戒', age:18}
//
// let add:{[key:string]:string,aaa:number}
//     add = {
//         name:'fds',
//         city:'we',
//         aaa:2
//     }
