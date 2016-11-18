/*
                PROBLEM MEMO
B::B               D90
EFC::EFC           F1C
EFCProxy::is_valid D90

#Problem
Bomberクラスのオブジェクトのアドレス==EFCProxy::is_validを呼ぶオブジェクトのアドレス

すなわち

BomberクラスのオブジェクトがEmergeFromCloudクラスのオブジェクトとして扱われている

すなわち

EmergeFromCloudをアダプトする時に、
add_memfunc内のラムダ式の中で使われるself(_state)が誤った
オブジェクトを指すポインタを返すことにつながっている



EFCProxy::is_validが2回呼ばれる
1度目はメインスレッドで正常に
2度目はTaskSystem::resume_allで呼ばれる←原因不明

子スレッドからメインスレッドで呼ばれるべき関数が呼ばれている？
↓
TODO: タスクシステムの見直しが必要


bom.is_validになぜかEFCProxy::is_validが登録されてる
<動く>
	local bom = Bomber.new(this_game, WW / 2, WH / 2)
	bom:is_valid() --BomberProxy::is_validが呼ばれる

<動かない>
	local bom = Bomber.new(this_game, WW / 2, WH / 2)
	local efc = EFC.new(this_game, 100, 100)
	bom:is_valid() --EFCProxy::is_validが呼ばれる

	bomとefcのメタテーブルの__indexが等しい
*/
