#include <boost/test/unit_test.hpp>

#include "../node.hpp"
#include "../parse.hpp"

struct ParserFixture {
  NodeFactory factory;
  const Node *nodeA;
  const Node *nodeB;

  ParserFixture() {
    nodeA = factory.buildNode("Aa");
    nodeB = factory.buildNode("Bb");
  }

  ~ParserFixture() {
    factory.deleteNodes();
  }

  void checkParse(const std::string& program, const Node *expectedParseTree) {
    const Node *actualParseTree = parse(program);
    BOOST_CHECK_EQUAL(*actualParseTree, *expectedParseTree);
    delete actualParseTree;
  }
};

BOOST_FIXTURE_TEST_SUITE( test_parser, ParserFixture )

BOOST_AUTO_TEST_CASE( empty_program_should_parse_to_i ) {
  const Node *nodeI = factory.buildNode("I");
  checkParse("I", nodeI);
}

BOOST_AUTO_TEST_CASE( single_token_should_parse_to_name_node ) {
  checkParse("Aa", nodeA);
  checkParse("Bb", nodeB);
}

BOOST_AUTO_TEST_CASE( two_tokens_should_parse_to_apply_node ) {
  const Node *nodeAB = factory.buildNode(*nodeA, *nodeB);
  const Node *nodeBA = factory.buildNode(*nodeB, *nodeA);
  checkParse("Aa Bb", nodeAB);
  checkParse("Bb Aa", nodeBA);
}

BOOST_AUTO_TEST_CASE( three_tokens_ungrouped_should_be_applied_left_to_right ) {
  const Node *nodeAB = factory.buildNode(*nodeA, *nodeB);
  const Node *nodeC = factory.buildNode("Cc");
  const Node *nodeABC = factory.buildNode(*nodeAB, *nodeC);
  checkParse("Aa Bb Cc", nodeABC);
}

BOOST_AUTO_TEST_CASE( three_tokens_with_first_two_grouped_should_be_applied_left_to_right ) {
  const Node *nodeAB = factory.buildNode(*nodeA, *nodeB);
  const Node *nodeC = factory.buildNode("Cc");
  const Node *nodeABC = factory.buildNode(*nodeAB, *nodeC);
  checkParse("(Aa Bb) Cc", nodeABC);
}

BOOST_AUTO_TEST_CASE( three_tokens_with_last_two_grouped_should_be_applied_right_to_left ) {
  const Node *nodeC = factory.buildNode("Cc");
  const Node *nodeBC = factory.buildNode(*nodeB, *nodeC);
  const Node *nodeABC = factory.buildNode(*nodeA, *nodeBC);
  checkParse("Aa (Bb Cc)", nodeABC);
}

BOOST_AUTO_TEST_CASE( four_tokens_with_pairs_grouped_should_parse_to_balanced_tree ) {
  const Node *nodeC = factory.buildNode("Cc");
  const Node *nodeD = factory.buildNode("Dd");

  const Node *nodeAB = factory.buildNode(*nodeA, *nodeB);
  const Node *nodeCD = factory.buildNode(*nodeC, *nodeD);
    
  const Node *nodeABCD = factory.buildNode(*nodeAB, *nodeCD);

  checkParse("(Aa Bb) (Cc Dd)", nodeABCD);
}

BOOST_AUTO_TEST_CASE( parser_should_ignore_unnecessary_whitespace_and_parens ) {
  const Node *nodeI = factory.buildNode("I");
  checkParse("()", nodeI);
  checkParse(" ", nodeI);

  checkParse(" Aa", nodeA);
  checkParse("Aa ", nodeA);
  checkParse("\tAa", nodeA);
  checkParse("( Aa )", nodeA);

  const Node *nodeAB = factory.buildNode(*nodeA, *nodeB);
  checkParse("Aa\tBb", nodeAB);
  checkParse("Aa ((Bb))", nodeAB);
  checkParse("(Aa)(Bb)", nodeAB);
  checkParse("Aa(Bb)", nodeAB);
  checkParse("(Aa)Bb", nodeAB);
  checkParse("(\t Aa  )\t\t( ((Bb)  )\t)", nodeAB);
}

BOOST_AUTO_TEST_CASE( parser_should_return_null_if_mismatched_parens ) {
  BOOST_CHECK( ! parse("(") );
  BOOST_CHECK( ! parse(") (") );
  BOOST_CHECK( ! parse(")") );
}

BOOST_AUTO_TEST_SUITE_END()
