if (false) {}
else if (type == "grids::message::services::List")
{
  msg = new grids::message::services::List();
  msg->ParseFromString(in_msg);
}else if (type == "grids::message::OTR::SMPRequest")
{
  msg = new grids::message::OTR::SMPRequest();
  msg->ParseFromString(in_msg);
}else if (type == "grids::message::node::Login")
{
  msg = new grids::message::node::Login();
  msg->ParseFromString(in_msg);
}else if (type == "grids::message::authentication::Login")
{
  msg = new grids::message::authentication::Login();
  msg->ParseFromString(in_msg);
}else if (type == "grids::message::program::CompileAndExecute")
{
  msg = new grids::message::program::CompileAndExecute();
  msg->ParseFromString(in_msg);
}else if (type == "grids::message::error::CompileError")
{
  msg = new grids::message::error::CompileError();
  msg->ParseFromString(in_msg);
}
else if (type == "grids::message::error::KeyNotFound")
{
  msg = new grids::message::error::KeyNotFound();
  msg->ParseFromString(in_msg);
}
else if (type == "grids::message::error::UnsupportedSerializer")
{
  msg = new grids::message::error::UnsupportedSerializer();
  msg->ParseFromString(in_msg);
}
else if (type == "grids::message::error::IncompatibleVersion")
{
  msg = new grids::message::error::IncompatibleVersion();
  msg->ParseFromString(in_msg);
}
else if (type == "grids::message::error::InvalidSerializer")
{
  msg = new grids::message::error::InvalidSerializer();
  msg->ParseFromString(in_msg);
}
else if (type == "grids::message::error::UnknownError")
{
  msg = new grids::message::error::UnknownError();
  msg->ParseFromString(in_msg);
}
else if (type == "grids::message::error::UnknownStatus")
{
  msg = new grids::message::error::UnknownStatus();
  msg->ParseFromString(in_msg);
}
else if (type == "grids::message::error::Unauthorized")
{
  msg = new grids::message::error::Unauthorized();
  msg->ParseFromString(in_msg);
}
else if (type == "grids::message::error::LoginInvalid")
{
  msg = new grids::message::error::LoginInvalid();
  msg->ParseFromString(in_msg);
}else if (type == "grids::message::storage::List")
{
  msg = new grids::message::storage::List();
  msg->ParseFromString(in_msg);
}
else if (type == "grids::message::storage::Put")
{
  msg = new grids::message::storage::Put();
  msg->ParseFromString(in_msg);
}
else if (type == "grids::message::storage::Delete")
{
  msg = new grids::message::storage::Delete();
  msg->ParseFromString(in_msg);
}
else if (type == "grids::message::storage::Get")
{
  msg = new grids::message::storage::Get();
  msg->ParseFromString(in_msg);
}
else if (type == "grids::message::storage::Empty")
{
  msg = new grids::message::storage::Empty();
  msg->ParseFromString(in_msg);
}else if (type == "grids::message::Encrypted")
{
  msg = new grids::message::Encrypted();
  msg->ParseFromString(in_msg);
}
else if (type == "grids::message::Unencrypted")
{
  msg = new grids::message::Unencrypted();
  msg->ParseFromString(in_msg);
}
else if (type == "grids::message::Echo")
{
  msg = new grids::message::Echo();
  msg->ParseFromString(in_msg);
}
else if (type == "grids::message::Connected")
{
  msg = new grids::message::Connected();
  msg->ParseFromString(in_msg);
}