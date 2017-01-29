/**************************************************************************
 ***                                                                    ***
 ***                          DeepC compiler                            ***
 ***                                                                    ***
 ***                       Parse tree generator                         ***
 ***                                                                    ***
 ***                                         - Zik Saleeba 2017         ***
 ***                                                                    ***
 **************************************************************************/

#ifndef CPARSERLISTENER_H
#define CPARSERLISTENER_H

#include "CListener.h"

class CParserListener : public CListener
{
public:
    CParserListener();
    
    void enterPrimaryExpression(CParser::PrimaryExpressionContext *ctx) override;
    void exitPrimaryExpression(CParser::PrimaryExpressionContext *ctx) override;
  
    void enterGenericSelection(CParser::GenericSelectionContext *ctx) override;
    void exitGenericSelection(CParser::GenericSelectionContext *ctx) override;
  
    void enterGenericAssocList(CParser::GenericAssocListContext *ctx) override;
    void exitGenericAssocList(CParser::GenericAssocListContext *ctx) override;
  
    void enterGenericAssociation(CParser::GenericAssociationContext *ctx) override;
    void exitGenericAssociation(CParser::GenericAssociationContext *ctx) override;
  
    void enterPostfixExpression(CParser::PostfixExpressionContext *ctx) override;
    void exitPostfixExpression(CParser::PostfixExpressionContext *ctx) override;
  
    void enterArgumentExpressionList(CParser::ArgumentExpressionListContext *ctx) override;
    void exitArgumentExpressionList(CParser::ArgumentExpressionListContext *ctx) override;
  
    void enterUnaryExpression(CParser::UnaryExpressionContext *ctx) override;
    void exitUnaryExpression(CParser::UnaryExpressionContext *ctx) override;
  
    void enterUnaryOperator(CParser::UnaryOperatorContext *ctx) override;
    void exitUnaryOperator(CParser::UnaryOperatorContext *ctx) override;
  
    void enterCastExpression(CParser::CastExpressionContext *ctx) override;
    void exitCastExpression(CParser::CastExpressionContext *ctx) override;
  
    void enterMultiplicativeExpression(CParser::MultiplicativeExpressionContext *ctx) override;
    void exitMultiplicativeExpression(CParser::MultiplicativeExpressionContext *ctx) override;
  
    void enterAdditiveExpression(CParser::AdditiveExpressionContext *ctx) override;
    void exitAdditiveExpression(CParser::AdditiveExpressionContext *ctx) override;
  
    void enterShiftExpression(CParser::ShiftExpressionContext *ctx) override;
    void exitShiftExpression(CParser::ShiftExpressionContext *ctx) override;
  
    void enterRelationalExpression(CParser::RelationalExpressionContext *ctx) override;
    void exitRelationalExpression(CParser::RelationalExpressionContext *ctx) override;
  
    void enterEqualityExpression(CParser::EqualityExpressionContext *ctx) override;
    void exitEqualityExpression(CParser::EqualityExpressionContext *ctx) override;
  
    void enterAndExpression(CParser::AndExpressionContext *ctx) override;
    void exitAndExpression(CParser::AndExpressionContext *ctx) override;
  
    void enterExclusiveOrExpression(CParser::ExclusiveOrExpressionContext *ctx) override;
    void exitExclusiveOrExpression(CParser::ExclusiveOrExpressionContext *ctx) override;
  
    void enterInclusiveOrExpression(CParser::InclusiveOrExpressionContext *ctx) override;
    void exitInclusiveOrExpression(CParser::InclusiveOrExpressionContext *ctx) override;
  
    void enterLogicalAndExpression(CParser::LogicalAndExpressionContext *ctx) override;
    void exitLogicalAndExpression(CParser::LogicalAndExpressionContext *ctx) override;
  
    void enterLogicalOrExpression(CParser::LogicalOrExpressionContext *ctx) override;
    void exitLogicalOrExpression(CParser::LogicalOrExpressionContext *ctx) override;
  
    void enterConditionalExpression(CParser::ConditionalExpressionContext *ctx) override;
    void exitConditionalExpression(CParser::ConditionalExpressionContext *ctx) override;
  
    void enterAssignmentExpression(CParser::AssignmentExpressionContext *ctx) override;
    void exitAssignmentExpression(CParser::AssignmentExpressionContext *ctx) override;
  
    void enterAssignmentOperator(CParser::AssignmentOperatorContext *ctx) override;
    void exitAssignmentOperator(CParser::AssignmentOperatorContext *ctx) override;
  
    void enterExpression(CParser::ExpressionContext *ctx) override;
    void exitExpression(CParser::ExpressionContext *ctx) override;
  
    void enterConstantExpression(CParser::ConstantExpressionContext *ctx) override;
    void exitConstantExpression(CParser::ConstantExpressionContext *ctx) override;
  
    void enterDeclaration(CParser::DeclarationContext *ctx) override;
    void exitDeclaration(CParser::DeclarationContext *ctx) override;
  
    void enterDeclarationSpecifiers(CParser::DeclarationSpecifiersContext *ctx) override;
    void exitDeclarationSpecifiers(CParser::DeclarationSpecifiersContext *ctx) override;
  
    void enterDeclarationSpecifiers2(CParser::DeclarationSpecifiers2Context *ctx) override;
    void exitDeclarationSpecifiers2(CParser::DeclarationSpecifiers2Context *ctx) override;
  
    void enterDeclarationSpecifier(CParser::DeclarationSpecifierContext *ctx) override;
    void exitDeclarationSpecifier(CParser::DeclarationSpecifierContext *ctx) override;
  
    void enterInitDeclaratorList(CParser::InitDeclaratorListContext *ctx) override;
    void exitInitDeclaratorList(CParser::InitDeclaratorListContext *ctx) override;
  
    void enterInitDeclarator(CParser::InitDeclaratorContext *ctx) override;
    void exitInitDeclarator(CParser::InitDeclaratorContext *ctx) override;
  
    void enterStorageClassSpecifier(CParser::StorageClassSpecifierContext *ctx) override;
    void exitStorageClassSpecifier(CParser::StorageClassSpecifierContext *ctx) override;
  
    void enterTypeSpecifier(CParser::TypeSpecifierContext *ctx) override;
    void exitTypeSpecifier(CParser::TypeSpecifierContext *ctx) override;
  
    void enterStructOrUnionSpecifier(CParser::StructOrUnionSpecifierContext *ctx) override;
    void exitStructOrUnionSpecifier(CParser::StructOrUnionSpecifierContext *ctx) override;
  
    void enterStructOrUnion(CParser::StructOrUnionContext *ctx) override;
    void exitStructOrUnion(CParser::StructOrUnionContext *ctx) override;
  
    void enterStructDeclarationList(CParser::StructDeclarationListContext *ctx) override;
    void exitStructDeclarationList(CParser::StructDeclarationListContext *ctx) override;
  
    void enterStructDeclaration(CParser::StructDeclarationContext *ctx) override;
    void exitStructDeclaration(CParser::StructDeclarationContext *ctx) override;
  
    void enterSpecifierQualifierList(CParser::SpecifierQualifierListContext *ctx) override;
    void exitSpecifierQualifierList(CParser::SpecifierQualifierListContext *ctx) override;
  
    void enterStructDeclaratorList(CParser::StructDeclaratorListContext *ctx) override;
    void exitStructDeclaratorList(CParser::StructDeclaratorListContext *ctx) override;
  
    void enterStructDeclarator(CParser::StructDeclaratorContext *ctx) override;
    void exitStructDeclarator(CParser::StructDeclaratorContext *ctx) override;
  
    void enterEnumSpecifier(CParser::EnumSpecifierContext *ctx) override;
    void exitEnumSpecifier(CParser::EnumSpecifierContext *ctx) override;
  
    void enterEnumeratorList(CParser::EnumeratorListContext *ctx) override;
    void exitEnumeratorList(CParser::EnumeratorListContext *ctx) override;
  
    void enterEnumerator(CParser::EnumeratorContext *ctx) override;
    void exitEnumerator(CParser::EnumeratorContext *ctx) override;
  
    void enterEnumerationConstant(CParser::EnumerationConstantContext *ctx) override;
    void exitEnumerationConstant(CParser::EnumerationConstantContext *ctx) override;
  
    void enterAtomicTypeSpecifier(CParser::AtomicTypeSpecifierContext *ctx) override;
    void exitAtomicTypeSpecifier(CParser::AtomicTypeSpecifierContext *ctx) override;
  
    void enterTypeQualifier(CParser::TypeQualifierContext *ctx) override;
    void exitTypeQualifier(CParser::TypeQualifierContext *ctx) override;
  
    void enterFunctionSpecifier(CParser::FunctionSpecifierContext *ctx) override;
    void exitFunctionSpecifier(CParser::FunctionSpecifierContext *ctx) override;
  
    void enterAlignmentSpecifier(CParser::AlignmentSpecifierContext *ctx) override;
    void exitAlignmentSpecifier(CParser::AlignmentSpecifierContext *ctx) override;
  
    void enterDeclarator(CParser::DeclaratorContext *ctx) override;
    void exitDeclarator(CParser::DeclaratorContext *ctx) override;
  
    void enterDirectDeclarator(CParser::DirectDeclaratorContext *ctx) override;
    void exitDirectDeclarator(CParser::DirectDeclaratorContext *ctx) override;
  
    void enterGccDeclaratorExtension(CParser::GccDeclaratorExtensionContext *ctx) override;
    void exitGccDeclaratorExtension(CParser::GccDeclaratorExtensionContext *ctx) override;
  
    void enterGccAttributeSpecifier(CParser::GccAttributeSpecifierContext *ctx) override;
    void exitGccAttributeSpecifier(CParser::GccAttributeSpecifierContext *ctx) override;
  
    void enterGccAttributeList(CParser::GccAttributeListContext *ctx) override;
    void exitGccAttributeList(CParser::GccAttributeListContext *ctx) override;
  
    void enterGccAttribute(CParser::GccAttributeContext *ctx) override;
    void exitGccAttribute(CParser::GccAttributeContext *ctx) override;
  
    void enterNestedParenthesesBlock(CParser::NestedParenthesesBlockContext *ctx) override;
    void exitNestedParenthesesBlock(CParser::NestedParenthesesBlockContext *ctx) override;
  
    void enterPointer(CParser::PointerContext *ctx) override;
    void exitPointer(CParser::PointerContext *ctx) override;
  
    void enterTypeQualifierList(CParser::TypeQualifierListContext *ctx) override;
    void exitTypeQualifierList(CParser::TypeQualifierListContext *ctx) override;
  
    void enterParameterTypeList(CParser::ParameterTypeListContext *ctx) override;
    void exitParameterTypeList(CParser::ParameterTypeListContext *ctx) override;
  
    void enterParameterList(CParser::ParameterListContext *ctx) override;
    void exitParameterList(CParser::ParameterListContext *ctx) override;
  
    void enterParameterDeclaration(CParser::ParameterDeclarationContext *ctx) override;
    void exitParameterDeclaration(CParser::ParameterDeclarationContext *ctx) override;
  
    void enterIdentifierList(CParser::IdentifierListContext *ctx) override;
    void exitIdentifierList(CParser::IdentifierListContext *ctx) override;
  
    void enterTypeName(CParser::TypeNameContext *ctx) override;
    void exitTypeName(CParser::TypeNameContext *ctx) override;
  
    void enterAbstractDeclarator(CParser::AbstractDeclaratorContext *ctx) override;
    void exitAbstractDeclarator(CParser::AbstractDeclaratorContext *ctx) override;
  
    void enterDirectAbstractDeclarator(CParser::DirectAbstractDeclaratorContext *ctx) override;
    void exitDirectAbstractDeclarator(CParser::DirectAbstractDeclaratorContext *ctx) override;
  
    void enterTypedefName(CParser::TypedefNameContext *ctx) override;
    void exitTypedefName(CParser::TypedefNameContext *ctx) override;
  
    void enterInitializer(CParser::InitializerContext *ctx) override;
    void exitInitializer(CParser::InitializerContext *ctx) override;
  
    void enterInitializerList(CParser::InitializerListContext *ctx) override;
    void exitInitializerList(CParser::InitializerListContext *ctx) override;
  
    void enterDesignation(CParser::DesignationContext *ctx) override;
    void exitDesignation(CParser::DesignationContext *ctx) override;
  
    void enterDesignatorList(CParser::DesignatorListContext *ctx) override;
    void exitDesignatorList(CParser::DesignatorListContext *ctx) override;
  
    void enterDesignator(CParser::DesignatorContext *ctx) override;
    void exitDesignator(CParser::DesignatorContext *ctx) override;
  
    void enterStaticAssertDeclaration(CParser::StaticAssertDeclarationContext *ctx) override;
    void exitStaticAssertDeclaration(CParser::StaticAssertDeclarationContext *ctx) override;
  
    void enterStatement(CParser::StatementContext *ctx) override;
    void exitStatement(CParser::StatementContext *ctx) override;
  
    void enterLabeledStatement(CParser::LabeledStatementContext *ctx) override;
    void exitLabeledStatement(CParser::LabeledStatementContext *ctx) override;
  
    void enterCompoundStatement(CParser::CompoundStatementContext *ctx) override;
    void exitCompoundStatement(CParser::CompoundStatementContext *ctx) override;
  
    void enterBlockItemList(CParser::BlockItemListContext *ctx) override;
    void exitBlockItemList(CParser::BlockItemListContext *ctx) override;
  
    void enterBlockItem(CParser::BlockItemContext *ctx) override;
    void exitBlockItem(CParser::BlockItemContext *ctx) override;
  
    void enterExpressionStatement(CParser::ExpressionStatementContext *ctx) override;
    void exitExpressionStatement(CParser::ExpressionStatementContext *ctx) override;
  
    void enterSelectionStatement(CParser::SelectionStatementContext *ctx) override;
    void exitSelectionStatement(CParser::SelectionStatementContext *ctx) override;
  
    void enterIterationStatement(CParser::IterationStatementContext *ctx) override;
    void exitIterationStatement(CParser::IterationStatementContext *ctx) override;
  
    void enterJumpStatement(CParser::JumpStatementContext *ctx) override;
    void exitJumpStatement(CParser::JumpStatementContext *ctx) override;
  
    void enterCompilationUnit(CParser::CompilationUnitContext *ctx) override;
    void exitCompilationUnit(CParser::CompilationUnitContext *ctx) override;
  
    void enterTranslationUnit(CParser::TranslationUnitContext *ctx) override;
    void exitTranslationUnit(CParser::TranslationUnitContext *ctx) override;
  
    void enterExternalDeclaration(CParser::ExternalDeclarationContext *ctx) override;
    void exitExternalDeclaration(CParser::ExternalDeclarationContext *ctx) override;
  
    void enterFunctionDefinition(CParser::FunctionDefinitionContext *ctx) override;
    void exitFunctionDefinition(CParser::FunctionDefinitionContext *ctx) override;
  
    void enterDeclarationList(CParser::DeclarationListContext *ctx) override;
    void exitDeclarationList(CParser::DeclarationListContext *ctx) override;    
    
    void visitTerminal(antlr4::tree::TerminalNode*) override;
    
    void visitErrorNode(antlr4::tree::ErrorNode*) override;
    
    void enterEveryRule(antlr4::ParserRuleContext*) override;
                     
    void exitEveryRule(antlr4::ParserRuleContext*) override;
};

#endif // CPARSERLISTENER_H
