/*************************************************************************
        > File Name: ec_param.c
        > Author:NEWPLAN
        > E-mail:newplan001@163.com
        > Created Time: Thu Apr 13 23:55:50 2017
 ************************************************************************/
#include "ec_param.h"
#include "xy_ecpoint.h"

/*
 *��ʼ����Բ����
 *����PABȷ������һ����Բ���ߵĲ���
 *y2=x3+ax+b�����������߷�ʽ��
 *p��������һ��ָ����F(p)�е�Ԫ�صĸ���
 *a��b ȷ��һ����Բ����
 *
 *n ����G�Ľ�(һ��Ҫ��Ϊ����)
 */
ec_param * ec_param_new(void)
{
	ec_param *ecp;
	ecp = (ec_param *)OPENSSL_malloc(sizeof(ec_param));
	/*����һ�����������Ļ���*/
	ecp->ctx = BN_CTX_new();
	ecp->p = BN_new();
	ecp->a = BN_new();
	ecp->b = BN_new();
	ecp->n = BN_new();
	return ecp;
}
//************************************
// Method:    ec_param_free
// FullName:  ec_param_free
// Access:    public
// Returns:   void
// Qualifier: �ͷſռ�
// Parameter: ec_param * ecp
//************************************
void ec_param_free(ec_param *ecp)
{
	if (ecp)
	{
		BN_free(ecp->p);
		ecp->p = NULL;
		BN_free(ecp->a);
		ecp->a = NULL;
		BN_free(ecp->b);
		ecp->b = NULL;
		BN_free(ecp->n);
		ecp->n = NULL;
		if (ecp->G)
		{
			xy_ecpoint_free(ecp->G);
			ecp->G = NULL;
		}
		if (ecp->group)
		{
			EC_GROUP_free(ecp->group);
			ecp->group = NULL;
		}
		BN_CTX_free(ecp->ctx);
		ecp->ctx = NULL;
		OPENSSL_free(ecp);
	}
}


//************************************
// Method:    ec_param_init
// FullName:  ec_param_init
// Access:    public
// Returns:   int
// Qualifier:��ʼ����Բ���ߵĲ���
// Parameter: ec_param * ecp ��Բ���߲����ṹ
// Parameter: char * * string_value Ҫ��ʼ����ֵ
// Parameter: int type ��Բ���ߵ����ͣ�������GFP���GF2M��
// Parameter: int point_bit_length ������ĳ���
//************************************
int ec_param_init(ec_param *ecp, char **string_value, int type, int point_bit_length)
{
	ecp->type = type;
	if (TYPE_GFp == ecp->type)
	{
		ecp->EC_GROUP_new_curve = EC_GROUP_new_curve_GFp;
		ecp->EC_POINT_set_affine_coordinates = EC_POINT_set_affine_coordinates_GFp;
		ecp->EC_POINT_get_affine_coordinates = EC_POINT_get_affine_coordinates_GFp;
	}
	else if (TYPE_GF2m == ecp->type)
	{
		ecp->EC_GROUP_new_curve = EC_GROUP_new_curve_GF2m;
		ecp->EC_POINT_set_affine_coordinates = EC_POINT_set_affine_coordinates_GF2m;
		ecp->EC_POINT_get_affine_coordinates = EC_POINT_get_affine_coordinates_GF2m;
	}

	/*hexת����big number*/
	BN_hex2bn(&ecp->p, string_value[0]);
	BN_hex2bn(&ecp->a, string_value[1]);
	BN_hex2bn(&ecp->b, string_value[2]);
	BN_hex2bn(&ecp->n, string_value[5]);

	/*��Կ����group�����Ⱥ�ĸ�����Ƕ�����������ɢ�ĵ�����Ӧ�Ĳ���*/
	ecp->group = ecp->EC_GROUP_new_curve(ecp->p, ecp->a
	                                     , ecp->b, ecp->ctx);
	/*��Բ�����Ļ���G*/
	ecp->G = xy_ecpoint_new(ecp);
	BN_hex2bn(&ecp->G->x, string_value[3]);
	BN_hex2bn(&ecp->G->y, string_value[4]);
	if (!ecp->EC_POINT_set_affine_coordinates(ecp->group
	        , ecp->G->ec_point, ecp->G->x
	        , ecp->G->y, ecp->ctx))
		ABORT
		/*��Բ���ߵĵ�ĳ���*/
		ecp->point_bit_length = point_bit_length;
	ecp->point_byte_length = (point_bit_length + 7) / 8;

	return SUCCESS;
}
