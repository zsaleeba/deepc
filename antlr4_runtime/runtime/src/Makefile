BINDIR  ?= ../../../bin
BUILD_DIR  ?= ../../../build
AROBJDIR=  $(BUILD_DIR)/deepc/antlr_runtime

CXXFLAGS += -g -std=c++11 -I. -Iatn -Idfa -Imisc -Isupport -Itree -Itree/pattern -Itree/xpath

TARGET	= $(AROBJDIR)/antlr_runtime.a
MAINSRCS = ANTLRFileStream.cpp ANTLRInputStream.cpp BailErrorStrategy.cpp BaseErrorListener.cpp \
	BufferedTokenStream.cpp CharStream.cpp CommonToken.cpp CommonTokenFactory.cpp \
	CommonTokenStream.cpp ConsoleErrorListener.cpp DefaultErrorStrategy.cpp \
	DiagnosticErrorListener.cpp Exceptions.cpp FailedPredicateException.cpp \
	InputMismatchException.cpp InterpreterRuleContext.cpp IntStream.cpp Lexer.cpp \
	LexerInterpreter.cpp LexerNoViableAltException.cpp ListTokenSource.cpp \
	NoViableAltException.cpp Parser.cpp ParserInterpreter.cpp ParserRuleContext.cpp \
	ProxyErrorListener.cpp RecognitionException.cpp Recognizer.cpp RuleContext.cpp \
	RuleContextWithAltNum.cpp RuntimeMetaData.cpp TokenStream.cpp TokenStreamRewriter.cpp \
	UnbufferedCharStream.cpp UnbufferedTokenStream.cpp Vocabulary.cpp
ATNSRCS = AbstractPredicateTransition.cpp ActionTransition.cpp AmbiguityInfo.cpp \
	ArrayPredictionContext.cpp ATNConfig.cpp ATNConfigSet.cpp ATN.cpp \
	ATNDeserializationOptions.cpp ATNDeserializer.cpp ATNSerializer.cpp ATNSimulator.cpp \
	ATNState.cpp AtomTransition.cpp BasicBlockStartState.cpp BasicState.cpp \
	BlockEndState.cpp ContextSensitivityInfo.cpp DecisionEventInfo.cpp DecisionInfo.cpp \
	DecisionState.cpp EmptyPredictionContext.cpp EpsilonTransition.cpp ErrorInfo.cpp \
	LexerActionExecutor.cpp LexerATNConfig.cpp LexerATNSimulator.cpp \
	LexerChannelAction.cpp LexerCustomAction.cpp LexerIndexedCustomAction.cpp \
	LexerModeAction.cpp LexerMoreAction.cpp LexerPopModeAction.cpp LexerPushModeAction.cpp \
	LexerSkipAction.cpp LexerTypeAction.cpp LL1Analyzer.cpp LookaheadEventInfo.cpp \
	LoopEndState.cpp NotSetTransition.cpp OrderedATNConfigSet.cpp ParseInfo.cpp \
	ParserATNSimulator.cpp PlusBlockStartState.cpp PlusLoopbackState.cpp \
	PrecedencePredicateTransition.cpp PredicateEvalInfo.cpp PredicateTransition.cpp \
	PredictionContext.cpp PredictionMode.cpp ProfilingATNSimulator.cpp RangeTransition.cpp \
	RuleStartState.cpp RuleStopState.cpp RuleTransition.cpp SemanticContext.cpp \
	SetTransition.cpp SingletonPredictionContext.cpp StarBlockStartState.cpp \
	StarLoopbackState.cpp StarLoopEntryState.cpp TokensStartState.cpp Transition.cpp \
	WildcardTransition.cpp
DFASRCS = DFA.cpp DFASerializer.cpp DFAState.cpp LexerDFASerializer.cpp
MISCSRCS = Interval.cpp IntervalSet.cpp MurmurHash.cpp
SUPPORTSRCS = Arrays.cpp CPPUtils.cpp guid.cpp StringUtils.cpp
TREESRCS = ErrorNodeImpl.cpp IterativeParseTreeWalker.cpp ParseTree.cpp ParseTreeWalker.cpp \
	TerminalNodeImpl.cpp Trees.cpp
PATTERNSRCS = ParseTreeMatch.cpp ParseTreePattern.cpp ParseTreePatternMatcher.cpp \
	RuleTagToken.cpp TagChunk.cpp TextChunk.cpp TokenTagToken.cpp
XPATHSRCS = XPath.cpp XPathElement.cpp XPathLexer.cpp XPathLexerErrorListener.cpp \
	XPathRuleAnywhereElement.cpp XPathRuleElement.cpp XPathTokenAnywhereElement.cpp \
	XPathTokenElement.cpp XPathWildcardAnywhereElement.cpp XPathWildcardElement.cpp
SRCS    := $(MAINSRCS) $(ATNSRCS:%.cpp=atn/%.cpp) $(DFASRCS:%.cpp=dfa/%.cpp) $(MISCSRCS:%.cpp=misc/%.cpp) $(SUPPORTSRCS:%.cpp=support/%.cpp) $(TREESRCS:%.cpp=tree/%.cpp) $(PATTERNSRCS:%.cpp=tree/pattern/%.cpp) $(XPATHSRCS:%.cpp=tree/xpath/%.cpp)
OBJS    := $(MAINSRCS:%.cpp=$(AROBJDIR)/%.o) $(ATNSRCS:%.cpp=$(AROBJDIR)/atn/%.o) $(DFASRCS:%.cpp=$(AROBJDIR)/dfa/%.o) $(MISCSRCS:%.cpp=$(AROBJDIR)/misc/%.o) $(SUPPORTSRCS:%.cpp=$(AROBJDIR)/support/%.o) $(TREESRCS:%.cpp=$(AROBJDIR)/tree/%.o) $(PATTERNSRCS:%.cpp=$(AROBJDIR)/tree/pattern/%.o) $(XPATHSRCS:%.cpp=$(AROBJDIR)/tree/xpath/%.o)

all:    $(AROBJDIR) $(TARGET)

$(TARGET): $(OBJS)
	$(AR) r $(TARGET) $(OBJS)

$(AROBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

$(AROBJDIR)/atn/%.o: atn/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

$(AROBJDIR)/dfa/%.o: dfa/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

$(AROBJDIR)/misc/%.o: misc/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

$(AROBJDIR)/support/%.o: support/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

$(AROBJDIR)/tree/%.o: tree/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

$(AROBJDIR)/tree/pattern/%.o: tree/pattern/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

$(AROBJDIR)/tree/xpath/%.o: tree/xpath/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	rm -f $(TARGET) $(OBJS)

$(AROBJDIR):
	mkdir -p $(AROBJDIR) $(AROBJDIR)/atn $(AROBJDIR)/dfa $(AROBJDIR)/misc $(AROBJDIR)/support $(AROBJDIR)/tree $(AROBJDIR)/tree/pattern $(AROBJDIR)/tree/xpath

.PHONY:	all clean
