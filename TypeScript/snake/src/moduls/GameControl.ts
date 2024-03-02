import ScorePanel from "./ScorePanel";
import Food from "./Food";
import Snake from "./Snake";

class GameControl{
    // 蛇
    snake: Snake;
    // 食物
    food: Food;
    // 记分牌
    scorePanel: ScorePanel;
    // 按键方向
    direction: string='';
    // 记录游戏是否结束
    isLive = true;


    constructor() {
        this.snake = new Snake();
        this.food = new Food();
        this.scorePanel = new ScorePanel(10,1);

        this.init();
    }

    // 游戏的初始化方法，调用后游戏即开始
    init(){
        // 绑定键盘按下事件
        document.addEventListener('keydown', this.keydownHandler.bind(this))
        this.run();
    }

    // 键盘按下响应函数
    keydownHandler(event: KeyboardEvent){
        this.direction = event.key;
    }

    run(){
        /**
         * 根据方向(this.direction) 来是蛇的位置改变
         *      向上 top 减少
         *      向下 top 增加
         *      向左 left 减少
         *      向右 left 增加
         */

        // 获取蛇现在的坐标
        let X = this.snake.X;
        let Y = this.snake.Y;

        // 根据按键方向，计算新坐标
        switch (this.direction){
            case "ArrowUp":
            case "Up":
                // 向上移动 top 减少
                Y -= 10;
                break;
            case "ArrowDown":
            case "Down":
                // 向下移动 top 增加
                Y += 10;
                break;
            case "ArrowLeft":
            case "Left":
                // 向左移动 left 减少
                X -= 10;
                break;
            case "ArrowRight":
            case "Right":
                // 向右移动 left 增加
                X += 10;
                break;
        }

        // 检查是否吃到食物
        this.checkEat(X, Y);

        try {// 修改蛇的坐标
            this.snake.X = X;
            this.snake.Y = Y;
        } catch (e){
            alert((e as Error).message + 'GAME OVER')
            this.isLive = false;
        }

        // 开启一个定时调用
        this.isLive && setTimeout(this.run.bind(this), 300 - (this.scorePanel.level-1) * 30)
    }


    // 检查是否吃到食物
    checkEat(X:number, Y:number){
        if(X === this.food.X && Y === this.food.Y) {
            console.log('吃到食物了')
            // 重置食物位置
            this.food.change();
            // 增加分数
            this.scorePanel.addScore();
            // 蛇身体增加一节
            this.snake.addBody();
        }
    }


}


export default GameControl;