/*
                PROBLEM MEMO
B::B               D90
EFC::EFC           F1C
EFCProxy::is_valid D90

#Problem
Bomber�N���X�̃I�u�W�F�N�g�̃A�h���X==EFCProxy::is_valid���ĂԃI�u�W�F�N�g�̃A�h���X

���Ȃ킿

Bomber�N���X�̃I�u�W�F�N�g��EmergeFromCloud�N���X�̃I�u�W�F�N�g�Ƃ��Ĉ����Ă���

���Ȃ킿

EmergeFromCloud���A�_�v�g���鎞�ɁA
add_memfunc���̃����_���̒��Ŏg����self(_state)�������
�I�u�W�F�N�g���w���|�C���^��Ԃ����ƂɂȂ����Ă���



EFCProxy::is_valid��2��Ă΂��
1�x�ڂ̓��C���X���b�h�Ő����
2�x�ڂ�TaskSystem::resume_all�ŌĂ΂�適�����s��

�q�X���b�h���烁�C���X���b�h�ŌĂ΂��ׂ��֐����Ă΂�Ă���H
��
TODO: �^�X�N�V�X�e���̌��������K�v


bom.is_valid�ɂȂ���EFCProxy::is_valid���o�^����Ă�
<����>
	local bom = Bomber.new(this_game, WW / 2, WH / 2)
	bom:is_valid() --BomberProxy::is_valid���Ă΂��

<�����Ȃ�>
	local bom = Bomber.new(this_game, WW / 2, WH / 2)
	local efc = EFC.new(this_game, 100, 100)
	bom:is_valid() --EFCProxy::is_valid���Ă΂��

	bom��efc�̃��^�e�[�u����__index��������
*/
