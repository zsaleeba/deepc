/**************************************************************************
 ***                                                                    ***
 ***                          DeepC compiler                            ***
 ***                                                                    ***
 ***                       Parse tree generator                         ***
 ***                                                                    ***
 ***                                         - Zik Saleeba 2017         ***
 ***                                                                    ***
 **************************************************************************/

#include "cparserlistener.h"


CParserListener::CParserListener()
{
}

void CParserListener::enterPrimaryExpression(CParser::PrimaryExpressionContext *ctx)
{
    std::cout << "CParserListener::enterPrimaryExpression()" << std::endl;
}

void CParserListener::exitPrimaryExpression(CParser::PrimaryExpressionContext *ctx)
{
    std::cout << "CParserListener::exitPrimaryExpression()" << std::endl;
}

void CParserListener::enterGenericSelection(CParser::GenericSelectionContext *ctx)
{
    std::cout << "CParserListener::enterGenericSelection()" << std::endl;
}

void CParserListener::exitGenericSelection(CParser::GenericSelectionContext *ctx)
{
    std::cout << "CParserListener::exitGenericSelection()" << std::endl;
}

void CParserListener::enterGenericAssocList(CParser::GenericAssocListContext *ctx)
{
    std::cout << "CParserListener::enterGenericAssocList()" << std::endl;
}

void CParserListener::exitGenericAssocList(CParser::GenericAssocListContext *ctx)
{
    std::cout << "CParserListener::exitGenericAssocList()" << std::endl;
}

void CParserListener::enterGenericAssociation(CParser::GenericAssociationContext *ctx)
{
    std::cout << "CParserListener::enterGenericAssociation()" << std::endl;
}

void CParserListener::exitGenericAssociation(CParser::GenericAssociationContext *ctx)
{
    std::cout << "CParserListener::exitGenericAssociation()" << std::endl;
}

void CParserListener::enterPostfixExpression(CParser::PostfixExpressionContext *ctx)
{
    std::cout << "CParserListener::enterPostfixExpression()" << std::endl;
}

void CParserListener::exitPostfixExpression(CParser::PostfixExpressionContext *ctx)
{
    std::cout << "CParserListener::exitPostfixExpression()" << std::endl;
}

void CParserListener::enterArgumentExpressionList(CParser::ArgumentExpressionListContext *ctx)
{
    std::cout << "CParserListener::enterArgumentExpressionList()" << std::endl;
}

void CParserListener::exitArgumentExpressionList(CParser::ArgumentExpressionListContext *ctx)
{
    std::cout << "CParserListener::exitArgumentExpressionList()" << std::endl;
}

void CParserListener::enterUnaryExpression(CParser::UnaryExpressionContext *ctx)
{
    std::cout << "CParserListener::enterUnaryExpression()" << std::endl;
}

void CParserListener::exitUnaryExpression(CParser::UnaryExpressionContext *ctx)
{
    std::cout << "CParserListener::exitUnaryExpression()" << std::endl;
}

void CParserListener::enterUnaryOperator(CParser::UnaryOperatorContext *ctx)
{
    std::cout << "CParserListener::enterUnaryOperator()" << std::endl;
}

void CParserListener::exitUnaryOperator(CParser::UnaryOperatorContext *ctx)
{
    std::cout << "CParserListener::exitUnaryOperator()" << std::endl;
}

void CParserListener::enterCastExpression(CParser::CastExpressionContext *ctx)
{
    std::cout << "CParserListener::enterCastExpression()" << std::endl;
}

void CParserListener::exitCastExpression(CParser::CastExpressionContext *ctx)
{
    std::cout << "CParserListener::exitCastExpression()" << std::endl;
}

void CParserListener::enterMultiplicativeExpression(CParser::MultiplicativeExpressionContext *ctx)
{
    std::cout << "CParserListener::enterMultiplicativeExpression()" << std::endl;
}

void CParserListener::exitMultiplicativeExpression(CParser::MultiplicativeExpressionContext *ctx)
{
    std::cout << "CParserListener::exitMultiplicativeExpression()" << std::endl;
}

void CParserListener::enterAdditiveExpression(CParser::AdditiveExpressionContext *ctx)
{
    std::cout << "CParserListener::enterAdditiveExpression()" << std::endl;
}

void CParserListener::exitAdditiveExpression(CParser::AdditiveExpressionContext *ctx)
{
    std::cout << "CParserListener::exitAdditiveExpression()" << std::endl;
}

void CParserListener::enterShiftExpression(CParser::ShiftExpressionContext *ctx)
{
    std::cout << "CParserListener::enterShiftExpression()" << std::endl;
}

void CParserListener::exitShiftExpression(CParser::ShiftExpressionContext *ctx)
{
    std::cout << "CParserListener::exitShiftExpression()" << std::endl;
}

void CParserListener::enterRelationalExpression(CParser::RelationalExpressionContext *ctx)
{
    std::cout << "CParserListener::enterRelationalExpression()" << std::endl;
}

void CParserListener::exitRelationalExpression(CParser::RelationalExpressionContext *ctx)
{
    std::cout << "CParserListener::exitRelationalExpression()" << std::endl;
}

void CParserListener::enterEqualityExpression(CParser::EqualityExpressionContext *ctx)
{
    std::cout << "CParserListener::enterEqualityExpression()" << std::endl;
}

void CParserListener::exitEqualityExpression(CParser::EqualityExpressionContext *ctx)
{
    std::cout << "CParserListener::exitEqualityExpression()" << std::endl;
}

void CParserListener::enterAndExpression(CParser::AndExpressionContext *ctx)
{
    std::cout << "CParserListener::enterAndExpression()" << std::endl;
}

void CParserListener::exitAndExpression(CParser::AndExpressionContext *ctx)
{
    std::cout << "CParserListener::exitAndExpression()" << std::endl;
}

void CParserListener::enterExclusiveOrExpression(CParser::ExclusiveOrExpressionContext *ctx)
{
    std::cout << "CParserListener::enterExclusiveOrExpression()" << std::endl;
}

void CParserListener::exitExclusiveOrExpression(CParser::ExclusiveOrExpressionContext *ctx)
{
    std::cout << "CParserListener::exitExclusiveOrExpression()" << std::endl;
}

void CParserListener::enterInclusiveOrExpression(CParser::InclusiveOrExpressionContext *ctx)
{
    std::cout << "CParserListener::enterInclusiveOrExpression()" << std::endl;
}

void CParserListener::exitInclusiveOrExpression(CParser::InclusiveOrExpressionContext *ctx)
{
    std::cout << "CParserListener::exitInclusiveOrExpression()" << std::endl;
}

void CParserListener::enterLogicalAndExpression(CParser::LogicalAndExpressionContext *ctx)
{
    std::cout << "CParserListener::enterLogicalAndExpression()" << std::endl;
}

void CParserListener::exitLogicalAndExpression(CParser::LogicalAndExpressionContext *ctx)
{
    std::cout << "CParserListener::exitLogicalAndExpression()" << std::endl;
}

void CParserListener::enterLogicalOrExpression(CParser::LogicalOrExpressionContext *ctx)
{
    std::cout << "CParserListener::enterLogicalOrExpression()" << std::endl;
}

void CParserListener::exitLogicalOrExpression(CParser::LogicalOrExpressionContext *ctx)
{
    std::cout << "CParserListener::exitLogicalOrExpression()" << std::endl;
}

void CParserListener::enterConditionalExpression(CParser::ConditionalExpressionContext *ctx)
{
    std::cout << "CParserListener::enterConditionalExpression()" << std::endl;
}

void CParserListener::exitConditionalExpression(CParser::ConditionalExpressionContext *ctx)
{
    std::cout << "CParserListener::exitConditionalExpression()" << std::endl;
}

void CParserListener::enterAssignmentExpression(CParser::AssignmentExpressionContext *ctx)
{
    std::cout << "CParserListener::enterAssignmentExpression()" << std::endl;
}

void CParserListener::exitAssignmentExpression(CParser::AssignmentExpressionContext *ctx)
{
    std::cout << "CParserListener::exitAssignmentExpression()" << std::endl;
}

void CParserListener::enterAssignmentOperator(CParser::AssignmentOperatorContext *ctx)
{
    std::cout << "CParserListener::enterAssignmentOperator()" << std::endl;
}

void CParserListener::exitAssignmentOperator(CParser::AssignmentOperatorContext *ctx)
{
    std::cout << "CParserListener::exitAssignmentOperator()" << std::endl;
}

void CParserListener::enterExpression(CParser::ExpressionContext *ctx)
{
    std::cout << "CParserListener::enterExpression()" << std::endl;
}

void CParserListener::exitExpression(CParser::ExpressionContext *ctx)
{
    std::cout << "CParserListener::exitExpression()" << std::endl;
}

void CParserListener::enterConstantExpression(CParser::ConstantExpressionContext *ctx)
{
    std::cout << "CParserListener::enterConstantExpression()" << std::endl;
}

void CParserListener::exitConstantExpression(CParser::ConstantExpressionContext *ctx)
{
    std::cout << "CParserListener::exitConstantExpression()" << std::endl;
}

void CParserListener::enterDeclaration(CParser::DeclarationContext *ctx)
{
    std::cout << "CParserListener::enterDeclaration()" << std::endl;
}

void CParserListener::exitDeclaration(CParser::DeclarationContext *ctx)
{
    std::cout << "CParserListener::exitDeclaration()" << std::endl;
}

void CParserListener::enterDeclarationSpecifiers(CParser::DeclarationSpecifiersContext *ctx)
{
    std::cout << "CParserListener::enterDeclarationSpecifiers()" << std::endl;
}

void CParserListener::exitDeclarationSpecifiers(CParser::DeclarationSpecifiersContext *ctx)
{
    std::cout << "CParserListener::exitDeclarationSpecifiers()" << std::endl;
}

void CParserListener::enterDeclarationSpecifiers2(CParser::DeclarationSpecifiers2Context *ctx)
{
    std::cout << "CParserListener::enterDeclarationSpecifiers2()" << std::endl;
}

void CParserListener::exitDeclarationSpecifiers2(CParser::DeclarationSpecifiers2Context *ctx)
{
    std::cout << "CParserListener::exitDeclarationSpecifiers2()" << std::endl;
}

void CParserListener::enterDeclarationSpecifier(CParser::DeclarationSpecifierContext *ctx)
{
    std::cout << "CParserListener::enterDeclarationSpecifier()" << std::endl;
}

void CParserListener::exitDeclarationSpecifier(CParser::DeclarationSpecifierContext *ctx)
{
    std::cout << "CParserListener::exitDeclarationSpecifier()" << std::endl;
}

void CParserListener::enterInitDeclaratorList(CParser::InitDeclaratorListContext *ctx)
{
    std::cout << "CParserListener::enterInitDeclaratorList()" << std::endl;
}

void CParserListener::exitInitDeclaratorList(CParser::InitDeclaratorListContext *ctx)
{
    std::cout << "CParserListener::exitInitDeclaratorList()" << std::endl;
}

void CParserListener::enterInitDeclarator(CParser::InitDeclaratorContext *ctx)
{
    std::cout << "CParserListener::enterInitDeclarator()" << std::endl;
}

void CParserListener::exitInitDeclarator(CParser::InitDeclaratorContext *ctx)
{
    std::cout << "CParserListener::exitInitDeclarator()" << std::endl;
}

void CParserListener::enterStorageClassSpecifier(CParser::StorageClassSpecifierContext *ctx)
{
    std::cout << "CParserListener::enterStorageClassSpecifier()" << std::endl;
}

void CParserListener::exitStorageClassSpecifier(CParser::StorageClassSpecifierContext *ctx)
{
    std::cout << "CParserListener::exitStorageClassSpecifier()" << std::endl;
}

void CParserListener::enterTypeSpecifier(CParser::TypeSpecifierContext *ctx)
{
    std::cout << "CParserListener::enterTypeSpecifier()" << std::endl;
}

void CParserListener::exitTypeSpecifier(CParser::TypeSpecifierContext *ctx)
{
    std::cout << "CParserListener::exitTypeSpecifier()" << std::endl;
}

void CParserListener::enterStructOrUnionSpecifier(CParser::StructOrUnionSpecifierContext *ctx)
{
    std::cout << "CParserListener::enterStructOrUnionSpecifier()" << std::endl;
}

void CParserListener::exitStructOrUnionSpecifier(CParser::StructOrUnionSpecifierContext *ctx)
{
    std::cout << "CParserListener::exitStructOrUnionSpecifier()" << std::endl;
}

void CParserListener::enterStructOrUnion(CParser::StructOrUnionContext *ctx)
{
    std::cout << "CParserListener::enterStructOrUnion()" << std::endl;
}

void CParserListener::exitStructOrUnion(CParser::StructOrUnionContext *ctx)
{
    std::cout << "CParserListener::exitStructOrUnion()" << std::endl;
}

void CParserListener::enterStructDeclarationList(CParser::StructDeclarationListContext *ctx)
{
    std::cout << "CParserListener::enterStructDeclarationList()" << std::endl;
}

void CParserListener::exitStructDeclarationList(CParser::StructDeclarationListContext *ctx)
{
    std::cout << "CParserListener::exitStructDeclarationList()" << std::endl;
}

void CParserListener::enterStructDeclaration(CParser::StructDeclarationContext *ctx)
{
    std::cout << "CParserListener::enterStructDeclaration()" << std::endl;
}

void CParserListener::exitStructDeclaration(CParser::StructDeclarationContext *ctx)
{
    std::cout << "CParserListener::exitStructDeclaration()" << std::endl;
}

void CParserListener::enterSpecifierQualifierList(CParser::SpecifierQualifierListContext *ctx)
{
    std::cout << "CParserListener::enterSpecifierQualifierList()" << std::endl;
}

void CParserListener::exitSpecifierQualifierList(CParser::SpecifierQualifierListContext *ctx)
{
    std::cout << "CParserListener::exitSpecifierQualifierList()" << std::endl;
}

void CParserListener::enterStructDeclaratorList(CParser::StructDeclaratorListContext *ctx)
{
    std::cout << "CParserListener::enterStructDeclaratorList()" << std::endl;
}

void CParserListener::exitStructDeclaratorList(CParser::StructDeclaratorListContext *ctx)
{
    std::cout << "CParserListener::exitStructDeclaratorList()" << std::endl;
}

void CParserListener::enterStructDeclarator(CParser::StructDeclaratorContext *ctx)
{
    std::cout << "CParserListener::enterStructDeclarator()" << std::endl;
}

void CParserListener::exitStructDeclarator(CParser::StructDeclaratorContext *ctx)
{
    std::cout << "CParserListener::exitStructDeclarator()" << std::endl;
}

void CParserListener::enterEnumSpecifier(CParser::EnumSpecifierContext *ctx)
{
    std::cout << "CParserListener::enterEnumSpecifier()" << std::endl;
}

void CParserListener::exitEnumSpecifier(CParser::EnumSpecifierContext *ctx)
{
    std::cout << "CParserListener::exitEnumSpecifier()" << std::endl;
}

void CParserListener::enterEnumeratorList(CParser::EnumeratorListContext *ctx)
{
    std::cout << "CParserListener::enterEnumeratorList()" << std::endl;
}

void CParserListener::exitEnumeratorList(CParser::EnumeratorListContext *ctx)
{
    std::cout << "CParserListener::exitEnumeratorList()" << std::endl;
}

void CParserListener::enterEnumerator(CParser::EnumeratorContext *ctx)
{
    std::cout << "CParserListener::enterEnumerator()" << std::endl;
}

void CParserListener::exitEnumerator(CParser::EnumeratorContext *ctx)
{
    std::cout << "CParserListener::exitEnumerator()" << std::endl;
}

void CParserListener::enterEnumerationConstant(CParser::EnumerationConstantContext *ctx)
{
    std::cout << "CParserListener::enterEnumerationConstant()" << std::endl;
}

void CParserListener::exitEnumerationConstant(CParser::EnumerationConstantContext *ctx)
{
    std::cout << "CParserListener::exitEnumerationConstant()" << std::endl;
}

void CParserListener::enterAtomicTypeSpecifier(CParser::AtomicTypeSpecifierContext *ctx)
{
    std::cout << "CParserListener::enterAtomicTypeSpecifier()" << std::endl;
}

void CParserListener::exitAtomicTypeSpecifier(CParser::AtomicTypeSpecifierContext *ctx)
{
    std::cout << "CParserListener::exitAtomicTypeSpecifier()" << std::endl;
}

void CParserListener::enterTypeQualifier(CParser::TypeQualifierContext *ctx)
{
    std::cout << "CParserListener::enterTypeQualifier()" << std::endl;
}

void CParserListener::exitTypeQualifier(CParser::TypeQualifierContext *ctx)
{
    std::cout << "CParserListener::exitTypeQualifier()" << std::endl;
}

void CParserListener::enterFunctionSpecifier(CParser::FunctionSpecifierContext *ctx)
{
    std::cout << "CParserListener::enterFunctionSpecifier()" << std::endl;
}

void CParserListener::exitFunctionSpecifier(CParser::FunctionSpecifierContext *ctx)
{
    std::cout << "CParserListener::exitFunctionSpecifier()" << std::endl;
}

void CParserListener::enterAlignmentSpecifier(CParser::AlignmentSpecifierContext *ctx)
{
    std::cout << "CParserListener::enterAlignmentSpecifier()" << std::endl;
}

void CParserListener::exitAlignmentSpecifier(CParser::AlignmentSpecifierContext *ctx)
{
    std::cout << "CParserListener::exitAlignmentSpecifier()" << std::endl;
}

void CParserListener::enterDeclarator(CParser::DeclaratorContext *ctx)
{
    std::cout << "CParserListener::enterDeclarator()" << std::endl;
}

void CParserListener::exitDeclarator(CParser::DeclaratorContext *ctx)
{
    std::cout << "CParserListener::exitDeclarator()" << std::endl;
}

void CParserListener::enterDirectDeclarator(CParser::DirectDeclaratorContext *ctx)
{
    std::cout << "CParserListener::enterDirectDeclarator()" << std::endl;
}

void CParserListener::exitDirectDeclarator(CParser::DirectDeclaratorContext *ctx)
{
    std::cout << "CParserListener::exitDirectDeclarator()" << std::endl;
}

void CParserListener::enterGccDeclaratorExtension(CParser::GccDeclaratorExtensionContext *ctx)
{
    std::cout << "CParserListener::enterGccDeclaratorExtension()" << std::endl;
}

void CParserListener::exitGccDeclaratorExtension(CParser::GccDeclaratorExtensionContext *ctx)
{
    std::cout << "CParserListener::exitGccDeclaratorExtension()" << std::endl;
}

void CParserListener::enterGccAttributeSpecifier(CParser::GccAttributeSpecifierContext *ctx)
{
    std::cout << "CParserListener::enterGccAttributeSpecifier()" << std::endl;
}

void CParserListener::exitGccAttributeSpecifier(CParser::GccAttributeSpecifierContext *ctx)
{
    std::cout << "CParserListener::exitGccAttributeSpecifier()" << std::endl;
}

void CParserListener::enterGccAttributeList(CParser::GccAttributeListContext *ctx)
{
    std::cout << "CParserListener::enterGccAttributeList()" << std::endl;
}

void CParserListener::exitGccAttributeList(CParser::GccAttributeListContext *ctx)
{
    std::cout << "CParserListener::exitGccAttributeList()" << std::endl;
}

void CParserListener::enterGccAttribute(CParser::GccAttributeContext *ctx)
{
    std::cout << "CParserListener::enterGccAttribute()" << std::endl;
}

void CParserListener::exitGccAttribute(CParser::GccAttributeContext *ctx)
{
    std::cout << "CParserListener::exitGccAttribute()" << std::endl;
}

void CParserListener::enterNestedParenthesesBlock(CParser::NestedParenthesesBlockContext *ctx)
{
    std::cout << "CParserListener::enterNestedParenthesesBlock()" << std::endl;
}

void CParserListener::exitNestedParenthesesBlock(CParser::NestedParenthesesBlockContext *ctx)
{
    std::cout << "CParserListener::exitNestedParenthesesBlock()" << std::endl;
}

void CParserListener::enterPointer(CParser::PointerContext *ctx)
{
    std::cout << "CParserListener::enterPointer()" << std::endl;
}

void CParserListener::exitPointer(CParser::PointerContext *ctx)
{
    std::cout << "CParserListener::exitPointer()" << std::endl;
}

void CParserListener::enterTypeQualifierList(CParser::TypeQualifierListContext *ctx)
{
    std::cout << "CParserListener::enterTypeQualifierList()" << std::endl;
}

void CParserListener::exitTypeQualifierList(CParser::TypeQualifierListContext *ctx)
{
    std::cout << "CParserListener::exitTypeQualifierList()" << std::endl;
}

void CParserListener::enterParameterTypeList(CParser::ParameterTypeListContext *ctx)
{
    std::cout << "CParserListener::enterParameterTypeList()" << std::endl;
}

void CParserListener::exitParameterTypeList(CParser::ParameterTypeListContext *ctx)
{
    std::cout << "CParserListener::exitParameterTypeList()" << std::endl;
}

void CParserListener::enterParameterList(CParser::ParameterListContext *ctx)
{
    std::cout << "CParserListener::enterParameterList()" << std::endl;
}

void CParserListener::exitParameterList(CParser::ParameterListContext *ctx)
{
    std::cout << "CParserListener::exitParameterList()" << std::endl;
}

void CParserListener::enterParameterDeclaration(CParser::ParameterDeclarationContext *ctx)
{
    std::cout << "CParserListener::enterParameterDeclaration()" << std::endl;
}

void CParserListener::exitParameterDeclaration(CParser::ParameterDeclarationContext *ctx)
{
    std::cout << "CParserListener::exitParameterDeclaration()" << std::endl;
}

void CParserListener::enterIdentifierList(CParser::IdentifierListContext *ctx)
{
    std::cout << "CParserListener::enterIdentifierList()" << std::endl;
}

void CParserListener::exitIdentifierList(CParser::IdentifierListContext *ctx)
{
    std::cout << "CParserListener::exitIdentifierList()" << std::endl;
}

void CParserListener::enterTypeName(CParser::TypeNameContext *ctx)
{
    std::cout << "CParserListener::enterTypeName()" << std::endl;
}

void CParserListener::exitTypeName(CParser::TypeNameContext *ctx)
{
    std::cout << "CParserListener::exitTypeName()" << std::endl;
}

void CParserListener::enterAbstractDeclarator(CParser::AbstractDeclaratorContext *ctx)
{
    std::cout << "CParserListener::enterAbstractDeclarator()" << std::endl;
}

void CParserListener::exitAbstractDeclarator(CParser::AbstractDeclaratorContext *ctx)
{
    std::cout << "CParserListener::exitAbstractDeclarator()" << std::endl;
}

void CParserListener::enterDirectAbstractDeclarator(CParser::DirectAbstractDeclaratorContext *ctx)
{
    std::cout << "CParserListener::enterDirectAbstractDeclarator()" << std::endl;
}

void CParserListener::exitDirectAbstractDeclarator(CParser::DirectAbstractDeclaratorContext *ctx)
{
    std::cout << "CParserListener::exitDirectAbstractDeclarator()" << std::endl;
}

void CParserListener::enterTypedefName(CParser::TypedefNameContext *ctx)
{
    std::cout << "CParserListener::enterTypedefName()" << std::endl;
}

void CParserListener::exitTypedefName(CParser::TypedefNameContext *ctx)
{
    std::cout << "CParserListener::exitTypedefName()" << std::endl;
}

void CParserListener::enterInitializer(CParser::InitializerContext *ctx)
{
    std::cout << "CParserListener::enterInitializer()" << std::endl;
}

void CParserListener::exitInitializer(CParser::InitializerContext *ctx)
{
    std::cout << "CParserListener::exitInitializer()" << std::endl;
}

void CParserListener::enterInitializerList(CParser::InitializerListContext *ctx)
{
    std::cout << "CParserListener::enterInitializerList()" << std::endl;
}

void CParserListener::exitInitializerList(CParser::InitializerListContext *ctx)
{
    std::cout << "CParserListener::exitInitializerList()" << std::endl;
}

void CParserListener::enterDesignation(CParser::DesignationContext *ctx)
{
    std::cout << "CParserListener::enterDesignation()" << std::endl;
}

void CParserListener::exitDesignation(CParser::DesignationContext *ctx)
{
    std::cout << "CParserListener::exitDesignation()" << std::endl;
}

void CParserListener::enterDesignatorList(CParser::DesignatorListContext *ctx)
{
    std::cout << "CParserListener::enterDesignatorList()" << std::endl;
}

void CParserListener::exitDesignatorList(CParser::DesignatorListContext *ctx)
{
    std::cout << "CParserListener::exitDesignatorList()" << std::endl;
}

void CParserListener::enterDesignator(CParser::DesignatorContext *ctx)
{
    std::cout << "CParserListener::enterDesignator()" << std::endl;
}

void CParserListener::exitDesignator(CParser::DesignatorContext *ctx)
{
    std::cout << "CParserListener::exitDesignator()" << std::endl;
}

void CParserListener::enterStaticAssertDeclaration(CParser::StaticAssertDeclarationContext *ctx)
{
    std::cout << "CParserListener::enterStaticAssertDeclaration()" << std::endl;
}

void CParserListener::exitStaticAssertDeclaration(CParser::StaticAssertDeclarationContext *ctx)
{
    std::cout << "CParserListener::exitStaticAssertDeclaration()" << std::endl;
}

void CParserListener::enterStatement(CParser::StatementContext *ctx)
{
    std::cout << "CParserListener::enterStatement()" << std::endl;
}

void CParserListener::exitStatement(CParser::StatementContext *ctx)
{
    std::cout << "CParserListener::exitStatement()" << std::endl;
}

void CParserListener::enterLabeledStatement(CParser::LabeledStatementContext *ctx)
{
    std::cout << "CParserListener::enterLabeledStatement()" << std::endl;
}

void CParserListener::exitLabeledStatement(CParser::LabeledStatementContext *ctx)
{
    std::cout << "CParserListener::exitLabeledStatement()" << std::endl;
}

void CParserListener::enterCompoundStatement(CParser::CompoundStatementContext *ctx)
{
    std::cout << "CParserListener::enterCompoundStatement()" << std::endl;
}

void CParserListener::exitCompoundStatement(CParser::CompoundStatementContext *ctx)
{
    std::cout << "CParserListener::exitCompoundStatement()" << std::endl;
}

void CParserListener::enterBlockItemList(CParser::BlockItemListContext *ctx)
{
    std::cout << "CParserListener::enterBlockItemList()" << std::endl;
}

void CParserListener::exitBlockItemList(CParser::BlockItemListContext *ctx)
{
    std::cout << "CParserListener::exitBlockItemList()" << std::endl;
}

void CParserListener::enterBlockItem(CParser::BlockItemContext *ctx)
{
    std::cout << "CParserListener::enterBlockItem()" << std::endl;
}

void CParserListener::exitBlockItem(CParser::BlockItemContext *ctx)
{
    std::cout << "CParserListener::exitBlockItem()" << std::endl;
}

void CParserListener::enterExpressionStatement(CParser::ExpressionStatementContext *ctx)
{
    std::cout << "CParserListener::enterExpressionStatement()" << std::endl;
}

void CParserListener::exitExpressionStatement(CParser::ExpressionStatementContext *ctx)
{
    std::cout << "CParserListener::exitExpressionStatement()" << std::endl;
}

void CParserListener::enterSelectionStatement(CParser::SelectionStatementContext *ctx)
{
    std::cout << "CParserListener::enterSelectionStatement()" << std::endl;
}

void CParserListener::exitSelectionStatement(CParser::SelectionStatementContext *ctx)
{
    std::cout << "CParserListener::exitSelectionStatement()" << std::endl;
}

void CParserListener::enterIterationStatement(CParser::IterationStatementContext *ctx)
{
    std::cout << "CParserListener::enterIterationStatement()" << std::endl;
}

void CParserListener::exitIterationStatement(CParser::IterationStatementContext *ctx)
{
    std::cout << "CParserListener::exitIterationStatement()" << std::endl;
}

void CParserListener::enterJumpStatement(CParser::JumpStatementContext *ctx)
{
    std::cout << "CParserListener::enterJumpStatement()" << std::endl;
}

void CParserListener::exitJumpStatement(CParser::JumpStatementContext *ctx)
{
    std::cout << "CParserListener::exitJumpStatement()" << std::endl;
}

void CParserListener::enterCompilationUnit(CParser::CompilationUnitContext *ctx)
{
    std::cout << "CParserListener::enterCompilationUnit()" << std::endl;
}

void CParserListener::exitCompilationUnit(CParser::CompilationUnitContext *ctx)
{
    std::cout << "CParserListener::exitCompilationUnit()" << std::endl;
}

void CParserListener::enterTranslationUnit(CParser::TranslationUnitContext *ctx)
{
    std::cout << "CParserListener::enterTranslationUnit()" << std::endl;
}

void CParserListener::exitTranslationUnit(CParser::TranslationUnitContext *ctx)
{
    std::cout << "CParserListener::exitTranslationUnit()" << std::endl;
}

void CParserListener::enterExternalDeclaration(CParser::ExternalDeclarationContext *ctx)
{
    std::cout << "CParserListener::enterExternalDeclaration()" << std::endl;
}

void CParserListener::exitExternalDeclaration(CParser::ExternalDeclarationContext *ctx)
{
    std::cout << "CParserListener::exitExternalDeclaration()" << std::endl;
}

void CParserListener::enterFunctionDefinition(CParser::FunctionDefinitionContext *ctx)
{
    std::cout << "CParserListener::enterFunctionDefinition()" << std::endl;
}

void CParserListener::exitFunctionDefinition(CParser::FunctionDefinitionContext *ctx)
{
    std::cout << "CParserListener::exitFunctionDefinition()" << std::endl;
}

void CParserListener::enterDeclarationList(CParser::DeclarationListContext *ctx)
{
    std::cout << "CParserListener::enterDeclarationList()" << std::endl;
}

void CParserListener::exitDeclarationList(CParser::DeclarationListContext *ctx)
{
    std::cout << "CParserListener::exitDeclarationList()" << std::endl;
}

void CParserListener::visitTerminal(antlr4::tree::TerminalNode*)
{
    std::cout << "CParserListener::visitTerminal()" << std::endl;
}

void CParserListener::visitErrorNode(antlr4::tree::ErrorNode*)
{
    std::cout << "CParserListener::visitErrorNode()" << std::endl;
}

void CParserListener::enterEveryRule(antlr4::ParserRuleContext*)
{
    std::cout << "CParserListener::enterEveryRule()" << std::endl;
}
                 
void CParserListener::exitEveryRule(antlr4::ParserRuleContext*)
{
    std::cout << "CParserListener::exitEveryRule()" << std::endl;
}
