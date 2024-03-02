
// 计分牌类
class ScorePanel {
    score = 0;
    level = 1;
    scoreEle: HTMLElement;
    levelEle: HTMLElement;
    // 等级限制
    maxLevel: number;
    // 表示多少分升一级
    upScore: number;


    constructor(maxLevel:number = 10, upScore:number = 10) {
        this.scoreEle = document.getElementById("score")!;
        this.levelEle = document.getElementById("level")!;

        this.maxLevel = maxLevel;
        this.upScore = upScore;
    }

    addScore(){
        // 加分
        this.scoreEle.innerHTML = ++this.score + '';

        if(this.score % this.upScore === 0){
            this.levelUp();
        }
    }

    levelUp(){
        // 等级提升
        if(this.level < this.maxLevel) {
            this.levelEle.innerHTML = ++this.level + '';
        }
    }
}
//
// const scorePanel = new ScorePanel();
// scorePanel.addScore();
// scorePanel.addScore();
// scorePanel.addScore();
// for (let i=0; i<100; i++){
//     scorePanel.addScore();
// }

export default ScorePanel;