class Snake{
    // 蛇头
    head: HTMLElement;
    // 蛇身(包括蛇头)
    bodies: HTMLCollection;
    // 蛇的容器
    element: HTMLElement;

    constructor() {
        this.element = document.getElementById('snake')!;
        this.head = document.querySelector('#snake > div') as HTMLElement;
        this.bodies = this.element.getElementsByTagName('div')
    }

    // 获取蛇头坐标
    get X(){
        return this.head.offsetLeft;
    }
    get Y(){
        return this.head.offsetTop;
    }

    // 设置蛇头坐标
    set X(value: number){
        if(this.X === value){
            return
        }

        // 合法范围 0-290之间
        if(value < 0 || value > 290){
            // 撞墙了
            throw new Error("撞墙了")
        }

        // 如果蛇身体存在第二个块，并且蛇头要设置的值的左边的坐标一致，则表示发生了水平方向掉头
        if(this.bodies[1] && (this.bodies[1] as HTMLElement).offsetLeft == value){
            console.log('发生水平方向掉头')
            if(value > this.X){
                // 蛇正在向左走，此时蛇想往右走，但若往右走就会穿透自己身体，所以要禁止
                value = this.X - 10;
            }else {
                value = this.X + 10;
            }
        }

        // 移动身体
        this.moveBody();

        this.head.style.left = value + 'px';
        // 检查是否相撞
        this.checkHeadBody();
    }

    set Y(value: number){
        if(this.Y === value){
            return
        }

        // 合法范围 0-290之间
        if(value < 0 || value > 290){
            // 撞墙了
            throw new Error("撞墙了")
        }

        // 如果蛇身体存在第二个块，并且蛇头要设置的值的top坐标一致，则表示发生了垂直方向掉头
        if(this.bodies[1] && (this.bodies[1] as HTMLElement).offsetTop == value){
            console.log('发生垂直方向掉头')
            if(value > this.Y){
                value = this.Y - 10;
            }else {
                value = this.Y + 10;
            }
        }

        // 移动身体
        this.moveBody();

        this.head.style.top = value + 'px';

        // 检查是否相撞
        this.checkHeadBody();
    }

    // 增加蛇身体
    addBody(): void{
        // 向element中添加一个div
        this.element.insertAdjacentHTML('beforeend',"<div></div>")
    }

    // 移动蛇身体
    moveBody(){
        for (let i = this.bodies.length-1; i > 0; i--) {
            // 获取前边身体的位置
            let X = (this.bodies[i-1] as HTMLElement).offsetLeft;
            let Y = (this.bodies[i-1] as HTMLElement).offsetTop;

            // 设置到当前身体上
            (this.bodies[i] as HTMLElement).style.left = X + 'px';
            (this.bodies[i] as HTMLElement).style.top = Y + 'px';

        }
    }

    // 检查蛇头和身体是否相撞
    checkHeadBody(){
        // 获取所有身体，检查是否和蛇头坐标重叠
        for (let i = 1; i < this.bodies.length; i++) {
            let bd = this.bodies[i] as HTMLElement;
            if(this.X === bd.offsetLeft && this.Y === bd.offsetTop){
                // 蛇头撞到了身体，游戏结束
                throw new Error('撞到自己了')
            }
        }
    }
}

export default Snake;