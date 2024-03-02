
class Food{
    element: HTMLElement;

    constructor() {
        // !表示忽略空值校验
        this.element = document.getElementById('food')!;
    }

    // 获取食物X坐标
    get X(){
        return this.element.offsetLeft;
    }

    get Y() {
        return this.element.offsetTop;
    }


    // 改变食物的位置
    change(){
        // 食物的位置，最小是0，最大是290
        // 移动一次是一格，一格是10，所以食物位置必须是10的倍数
        let left = Math.round(Math.random() * 29) * 10;
        let top = Math.round(Math.random() * 29) * 10;

        this.element.style.left = left+'px';
        this.element.style.top = top+'px'
    }
}
//
// const food = new Food();
// console.log(food.X, food.Y)
//
// food.change()
// console.log(food.X, food.Y)

export default Food;