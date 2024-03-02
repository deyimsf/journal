console.log("hello TS");
// let a: number;
// a = 10;
// // a = 'hello'
// let b:string;
// b = "hello"
// let c = true;
//
// function sum(a:number, b:number): number{
//     return a + b;
// }
//
// sum(123,456)
//
// // 联合类型
// let abc: boolean | string
//
// // any表示任意类型，表示这个变量关闭了TS的类型检查
// let d: any
//
// // 未知类型
// let e: unknown
//
// // 类型断言
// a = e as number;
//
// function fun(): never {
//     throw new Error('报错了');
// }
let f;
f = { name: "fd" };
let e;
let g;
// 元组,是固定长度的数组
// 枚举
var Gender;
(function (Gender) {
    Gender[Gender["Male"] = 0] = "Male";
    Gender[Gender["Female"] = 1] = "Female";
})(Gender || (Gender = {}));
let i;
i = {
    name: "张三",
    gender: Gender.Female
};
console.log(i.gender === Gender.Female);
let k;
k = 2;
k = 'h';
