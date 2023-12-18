#include <iostream>

#include "Misc/AutomationTest.h"
#include "MaterialInstanceUtil.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MIIsMIDDerivedFromTest, "TPSGame1.TPSGame1.MidDerivedTest",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MIIsMIDDerivedFromTest::RunTest(const FString& Parameters)
{
	// Make the test pass by returning true, or fail by returning false.
	
	auto *M_Parent = Cast<UMaterialInterface>(
		StaticLoadObject(UMaterialInterface::StaticClass(), nullptr,TEXT("Material'/Game/Tests/M_Parent.M_Parent'"))
	);
	
	auto *MI_Red = Cast<UMaterialInterface>(
		StaticLoadObject(UMaterialInterface::StaticClass(), nullptr,TEXT("MaterialInstanceConstant'/Game/Tests/MI_Red.MI_Red'"))
	);
	
	auto *MI_Red1 = Cast<UMaterialInterface>(
		StaticLoadObject(UMaterialInterface::StaticClass(), nullptr,TEXT("MaterialInstanceConstant'/Game/Tests/MI_Red_1.MI_Red_1'"))
	);

	auto *MI_Green = Cast<UMaterialInterface>(
		StaticLoadObject(UMaterialInterface::StaticClass(), nullptr,TEXT("MaterialInstanceConstant'/Game/Tests/MI_Green.MI_Green'"))
	);

	auto *MID = UMaterialInstanceDynamic::Create(MI_Red1, GetTransientPackage());
	MID->SetScalarParameterValue(TEXT("G"), 0.f);

	UMaterialInterface* TestCases[][2]={
		{M_Parent, M_Parent},
		{MI_Red,MI_Red,},
		{MI_Red1,MI_Red1,},
		{MID, MID},
		{MI_Red, M_Parent},
		{MI_Red1, MI_Red},
		{MI_Red1, M_Parent},
		{MID, M_Parent},
		{MID, MI_Red},
		{MID, MI_Red1},
		{MID, MI_Green}
	};

	short expected[] = {
		0,0,0,1, 0,0,0, 1,1,1,0
	};

	for (int i=0;i < 11;++i)
	{
		if (expected[i])
		{
			TestTrue(FString::FromInt(i), UMaterialInstanceUtil::IsMIDDerivedFrom(TestCases[i][0], TestCases[i][1]));
		}
		else
		{
			TestFalse(FString::FromInt(i), UMaterialInstanceUtil::IsMIDDerivedFrom(TestCases[i][0], TestCases[i][1]));
		}
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MIIsParameterInheritedFromTest, "TPSGame1.TPSGame1.ParameterInheritedTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MIIsParameterInheritedFromTest::RunTest(const FString& Parameters)
{

	auto *M_Parent = Cast<UMaterialInterface>(
		StaticLoadObject(UMaterialInterface::StaticClass(), nullptr,TEXT("Material'/Game/Tests/M_Parent.M_Parent'"))
	);
	
	auto *MI_Red = Cast<UMaterialInterface>(
		StaticLoadObject(UMaterialInterface::StaticClass(), nullptr,TEXT("MaterialInstanceConstant'/Game/Tests/MI_Red.MI_Red'"))
	);
	
	auto *MI_Red1 = Cast<UMaterialInterface>(
		StaticLoadObject(UMaterialInterface::StaticClass(), nullptr,TEXT("MaterialInstanceConstant'/Game/Tests/MI_Red_1.MI_Red_1'"))
	);
	
	auto *MI_Red2 = Cast<UMaterialInterface>(
		StaticLoadObject(UMaterialInterface::StaticClass(), nullptr,TEXT("MaterialInstanceConstant'/Game/Tests/MI_Red_2.MI_Red_2'"))
	);

	auto *MI_Green = Cast<UMaterialInterface>(
		StaticLoadObject(UMaterialInterface::StaticClass(), nullptr,TEXT("MaterialInstanceConstant'/Game/Tests/MI_Green.MI_Green'"))
	);

	auto *MID = UMaterialInstanceDynamic::Create(MI_Red2, GetTransientPackage(), TEXT("MID_Red"));
	MID->SetScalarParameterValue(TEXT("B"), 0.f);
	
	auto *MID_Pa = UMaterialInstanceDynamic::Create(M_Parent, GetTransientPackage(), TEXT("MID_Parent"));
	MID_Pa->SetScalarParameterValue(TEXT("G"), 0.f);

	auto r = TEXT("R"), g = TEXT("G"), b = TEXT("B"), a = TEXT("A");
	
	{
		UMaterialInterface* MI_pairs[][2] = {
			{M_Parent, M_Parent},
			{MI_Red,MI_Red,},
			{MID, MID},
		
			{MI_Red, MI_Red1},
			{MI_Red1, MI_Green},
			{MI_Red2, MID},
		};

		for (int i=0;i < 3;++i)
		{
			auto p1 = MI_pairs[i][0], p2 = MI_pairs[i][1];
			for (auto arg: {r,g,b})
			{
				TestTrue(FString::Printf(TEXT("%s self: %s"),*p1->GetName(), arg),
					UMaterialInstanceUtil::IsParameterInheritedFrom(arg, p1,p2));
			}
			TestFalse(FString::Printf(TEXT("%s self: A"),*p1->GetName()),
				UMaterialInstanceUtil::IsParameterInheritedFrom(TEXT("A"), p1,p2));
		}
	
		for (int i=3;i < 6;++i)
		{
			auto p1 = MI_pairs[i][0], p2 = MI_pairs[i][1];
			for (auto arg: {r,g,b,a})
			{
				TestFalse(FString::Printf(TEXT("%s and %s: %s"),*p1->GetName(), *p2->GetName(),arg),
					UMaterialInstanceUtil::IsParameterInheritedFrom(arg, p1,p2));
			}
		}
	}
	{
		UMaterialInterface* MI_pairs[][2] =  {
			{MID_Pa, M_Parent},
			{MI_Red, M_Parent},
			{MI_Red1, M_Parent},
			{MI_Red1, MI_Red},
			{MI_Red2, M_Parent},
			{MI_Red2, MI_Red},
			{MI_Red2, MI_Red1},
			{MID, M_Parent},
			{MID, MI_Red},
			{MID, MI_Red1},
			{MID, MI_Red2},
		};

		short expected[][3] = {
			1,0,1, 1,0,0, 1,0,0, 1,1,0,
			0,0,0, 0,1,0, 0,1,0,
			0,0,0, 0,1,0, 0,1,0, 1,1,0
		};
		
		for (int i=0;i < 11;++i)
		{
			auto p1 = MI_pairs[i][0], p2 = MI_pairs[i][1];
			const TCHAR* args[] = {r,g,b};
			for (int j=0;j < 3;++j)
			{
				auto message = FString::Printf(TEXT("%s and %s: %s"),*p1->GetName(), *p2->GetName(),args[j]);
				UE_LOG(LogTemp, Display, TEXT("%s expect %d"), *message, expected[i][j]);
				if (expected[i][j])
				{
					TestTrue(message,UMaterialInstanceUtil::IsParameterInheritedFrom(args[j], p1,p2));
				}
				else
				{
					TestFalse(message,UMaterialInstanceUtil::IsParameterInheritedFrom(args[j], p1,p2));
				}
			}
		}
	}
	
	return true;
}

