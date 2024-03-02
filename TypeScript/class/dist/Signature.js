"use strict";
let array1 = ['a', 'b'];
let array2 = { 0: 'a', 1: 'b' };
console.log(array1[0]);
console.log(array2[0]);
const abcObj = {
    age: 18,
    fd: 4,
    name: '张三',
    check(n) {
        return 1;
    }
};
console.log(abcObj.fd, abcObj.check(3));
//
// class MyClass implements MyInter{
//     age:number=1;
//     name:string='张三';
//     // fd=4;
// }
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
